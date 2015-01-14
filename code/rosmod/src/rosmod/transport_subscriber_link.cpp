#include "rosmod/transport_subscriber_link.h"
#include "rosmod/publication.h"
#include "ros/header.h"
#include "ros/connection.h"
#include "ros/transport/transport.h"
#include "ros/this_node.h"
#include "ros/connection_manager.h"
#include "rosmod/topic_manager.h"
#include "ros/file_log.h"

#include <boost/bind.hpp>

namespace ros
{

TransportSubscriberLink2::TransportSubscriberLink2()
: writing_message_(false)
, header_written_(false)
, queue_full_(false)
{

}

TransportSubscriberLink2::~TransportSubscriberLink2()
{
  drop();
}

bool TransportSubscriberLink2::initialize(const ConnectionPtr& connection)
{
  connection_ = connection;
  dropped_conn_ = connection_->addDropListener(boost::bind(&TransportSubscriberLink2::onConnectionDropped, this, _1));

  return true;
}

bool TransportSubscriberLink2::handleHeader(const Header& header)
{
  std::string topic;
  if (!header.getValue("topic", topic))
  {
    std::string msg("Header from subscriber did not have the required element: topic");

    ROS_ERROR("%s", msg.c_str());
    connection_->sendHeaderError(msg);

    return false;
  }

  // This will get validated by validateHeader below
  std::string client_callerid;
  header.getValue("callerid", client_callerid);
  PublicationPtr2 pt = TopicManager2::instance()->lookupPublication(topic);
  if (!pt)
  {
    std::string msg = std::string("received a connection for a nonexistent topic [") +
                    topic + std::string("] from [" + connection_->getTransport()->getTransportInfo() + "] [" + client_callerid +"].");

    ROSCPP_LOG_DEBUG("%s", msg.c_str());
    connection_->sendHeaderError(msg);

    return false;
  }

  std::string error_msg;
  if (!pt->validateHeader(header, error_msg))
  {
    ROSCPP_LOG_DEBUG("%s", error_msg.c_str());
    connection_->sendHeaderError(error_msg);

    return false;
  }

  destination_caller_id_ = client_callerid;
  connection_id_ = ConnectionManager::instance()->getNewConnectionID();
  topic_ = pt->getName();
  parent_ = PublicationWPtr2(pt);

  // Send back a success, with info
  M_string m;
  m["type"] = pt->getDataType();
  m["md5sum"] = pt->getMD5Sum();
  m["message_definition"] = pt->getMessageDefinition();
  m["callerid"] = this_node::getName();
  m["latching"] = pt->isLatching() ? "1" : "0";
  connection_->writeHeader(m, boost::bind(&TransportSubscriberLink2::onHeaderWritten, this, _1));

  pt->addSubscriberLink(shared_from_this());

  return true;
}

void TransportSubscriberLink2::onConnectionDropped(const ConnectionPtr& conn)
{
  ROS_ASSERT(conn == connection_);

  PublicationPtr2 parent = parent_.lock();

  if (parent)
  {
    ROSCPP_LOG_DEBUG("Connection to subscriber [%s] to topic [%s] dropped", connection_->getRemoteString().c_str(), topic_.c_str());

    parent->removeSubscriberLink(shared_from_this());
  }
}

void TransportSubscriberLink2::onHeaderWritten(const ConnectionPtr& conn)
{
  header_written_ = true;
  startMessageWrite(true);
}

void TransportSubscriberLink2::onMessageWritten(const ConnectionPtr& conn)
{
  writing_message_ = false;
  startMessageWrite(true);
}

void TransportSubscriberLink2::startMessageWrite(bool immediate_write)
{
  boost::shared_array<uint8_t> dummy;
  SerializedMessage m(dummy, (uint32_t)0);

  {
    boost::mutex::scoped_lock lock(outbox_mutex_);
    if (writing_message_ || !header_written_)
    {
      return;
    }

    if (!outbox_.empty())
    {
      writing_message_ = true;
      m = outbox_.front();
      outbox_.pop();
    }
  }

  if (m.num_bytes > 0)
  {
    connection_->write(m.buf, m.num_bytes, boost::bind(&TransportSubscriberLink2::onMessageWritten, this, _1), immediate_write);
  }
}

void TransportSubscriberLink2::enqueueMessage(const SerializedMessage& m, bool ser, bool nocopy)
{
  if (!ser)
  {
    return;
  }

  {
    boost::mutex::scoped_lock lock(outbox_mutex_);

    int max_queue = 0;
    if (PublicationPtr2 parent = parent_.lock())
    {
      max_queue = parent->getMaxQueue();
    }

    ROS_DEBUG_NAMED("superdebug", "TransportSubscriberLink on topic [%s] to caller [%s], queueing message (queue size [%d])", topic_.c_str(), destination_caller_id_.c_str(), (int)outbox_.size());

    if (max_queue > 0 && (int)outbox_.size() >= max_queue)
    {
      if (!queue_full_)
      {
        ROS_DEBUG("Outgoing queue full for topic [%s].  "
               "Discarding oldest message\n",
               topic_.c_str());
      }

      outbox_.pop(); // toss out the oldest thing in the queue to make room for us
      queue_full_ = true;
    }
    else
    {
      queue_full_ = false;
    }

    outbox_.push(m);
  }

  startMessageWrite(false);

  stats_.messages_sent_++;
  stats_.bytes_sent_ += m.num_bytes;
  stats_.message_data_sent_ += m.num_bytes;
}

std::string TransportSubscriberLink2::getTransportType()
{
  return connection_->getTransport()->getType();
}

std::string TransportSubscriberLink2::getTransportInfo()
{
  return connection_->getTransport()->getTransportInfo();
}

void TransportSubscriberLink2::drop()
{
  // Only drop the connection if it's not already sending a header error
  // If it is, it will automatically drop itself
  if (connection_->isSendingHeaderError())
  {
    connection_->removeDropListener(dropped_conn_);
  }
  else
  {
    connection_->drop(Connection::Destructing);
  }
}

} // namespace ros
