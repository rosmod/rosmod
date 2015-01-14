#include "rosmod/intraprocess_subscriber_link.h"
#include "rosmod/intraprocess_publisher_link.h"
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

IntraProcessSubscriberLink2::IntraProcessSubscriberLink2(const PublicationPtr2& parent)
: dropped_(false)
{
  ROS_ASSERT(parent);
  parent_ = parent;
  topic_ = parent->getName();
}

IntraProcessSubscriberLink2::~IntraProcessSubscriberLink2()
{
}

void IntraProcessSubscriberLink2::setSubscriber(const IntraProcessPublisherLinkPtr2& subscriber)
{
  subscriber_ = subscriber;
  connection_id_ = ConnectionManager::instance()->getNewConnectionID();
  destination_caller_id_ = this_node::getName();
}

bool IntraProcessSubscriberLink2::isLatching()
{
  if (PublicationPtr2 parent = parent_.lock())
  {
    return parent->isLatching();
  }

  return false;
}

void IntraProcessSubscriberLink2::enqueueMessage(const SerializedMessage& m, bool ser, bool nocopy)
{
  boost::recursive_mutex::scoped_lock lock(drop_mutex_);
  if (dropped_)
  {
    return;
  }

  ROS_ASSERT(subscriber_);
  subscriber_->handleMessage(m, ser, nocopy);
}

std::string IntraProcessSubscriberLink2::getTransportType()
{
  return std::string("INTRAPROCESS");
}

std::string IntraProcessSubscriberLink2::getTransportInfo()
{
  // TODO: Check if we can dump more useful information here
  return getTransportType();
}

void IntraProcessSubscriberLink2::drop()
{
  {
    boost::recursive_mutex::scoped_lock lock(drop_mutex_);
    if (dropped_)
    {
      return;
    }

    dropped_ = true;
  }

  if (subscriber_)
  {
    subscriber_->drop();
    subscriber_.reset();
  }

  if (PublicationPtr2 parent = parent_.lock())
  {
    ROSCPP_LOG_DEBUG("Connection to local subscriber on topic [%s] dropped", topic_.c_str());

    parent->removeSubscriberLink(shared_from_this());
  }
}

void IntraProcessSubscriberLink2::getPublishTypes(bool& ser, bool& nocopy, const std::type_info& ti)
{
  boost::recursive_mutex::scoped_lock lock(drop_mutex_);
  if (dropped_)
  {
    return;
  }

  subscriber_->getPublishTypes(ser, nocopy, ti);
}

} // namespace ros
