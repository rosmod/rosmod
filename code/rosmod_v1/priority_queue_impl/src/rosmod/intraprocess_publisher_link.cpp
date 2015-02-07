#include "rosmod/intraprocess_publisher_link.h"
#include "rosmod/intraprocess_subscriber_link.h"
#include "rosmod/subscription.h"
#include "ros/header.h"
#include "ros/connection.h"
#include "ros/transport/transport.h"
#include "ros/this_node.h"
#include "ros/connection_manager.h"
#include "ros/file_log.h"

#include <boost/bind.hpp>

#include <sstream>

namespace ros
{

IntraProcessPublisherLink2::IntraProcessPublisherLink2(const SubscriptionPtr2& parent, const std::string& xmlrpc_uri, const TransportHints& transport_hints)
: PublisherLink2(parent, xmlrpc_uri, transport_hints)
, dropped_(false)
{
}

IntraProcessPublisherLink2::~IntraProcessPublisherLink2()
{
}

void IntraProcessPublisherLink2::setPublisher(const IntraProcessSubscriberLinkPtr2& publisher)
{
  publisher_ = publisher;

  SubscriptionPtr2 parent = parent_.lock();
  ROS_ASSERT(parent);

  Header header;
  M_stringPtr values = header.getValues();
  (*values)["callerid"] = this_node::getName();
  (*values)["topic"] = parent->getName();
  (*values)["type"] = publisher->getDataType();
  (*values)["md5sum"] = publisher->getMD5Sum();
  (*values)["message_definition"] = publisher->getMessageDefinition();
  (*values)["latching"] = publisher->isLatching() ? "1" : "0";
  setHeader(header);
}

void IntraProcessPublisherLink2::drop()
{
  {
    boost::recursive_mutex::scoped_lock lock(drop_mutex_);
    if (dropped_)
    {
      return;
    }

    dropped_ = true;
  }

  if (publisher_)
  {
    publisher_->drop();
    publisher_.reset();
  }

  if (SubscriptionPtr2 parent = parent_.lock())
  {
    ROSCPP_LOG_DEBUG("Connection to local publisher on topic [%s] dropped", parent->getName().c_str());

    parent->removePublisherLink(shared_from_this());
  }
}

void IntraProcessPublisherLink2::handleMessage(const SerializedMessage& m, bool ser, bool nocopy)
{
  boost::recursive_mutex::scoped_lock lock(drop_mutex_);
  if (dropped_)
  {
    return;
  }

  stats_.bytes_received_ += m.num_bytes;
  stats_.messages_received_++;

  SubscriptionPtr2 parent = parent_.lock();

  if (parent)
  {
    stats_.drops_ += parent->handleMessage(m, ser, nocopy, header_.getValues(), shared_from_this());
  }
}

std::string IntraProcessPublisherLink2::getTransportType()
{
  return std::string("INTRAPROCESS");
}

std::string IntraProcessPublisherLink2::getTransportInfo()
{
  // TODO: Check if we can dump more useful information here
  return getTransportType();
}

void IntraProcessPublisherLink2::getPublishTypes(bool& ser, bool& nocopy, const std::type_info& ti)
{
  boost::recursive_mutex::scoped_lock lock(drop_mutex_);
  if (dropped_)
  {
    ser = false;
    nocopy = false;
    return;
  }

  SubscriptionPtr2 parent = parent_.lock();
  if (parent)
  {
    parent->getPublishTypes(ser, nocopy, ti);
  }
  else
  {
    ser = true;
    nocopy = false;
  }
}

} // namespace ros

