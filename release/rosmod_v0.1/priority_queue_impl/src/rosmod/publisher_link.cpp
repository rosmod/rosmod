#include "rosmod/publisher_link.h"
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

PublisherLink2::PublisherLink2(const SubscriptionPtr2& parent, const std::string& xmlrpc_uri, 
			     const TransportHints& transport_hints)
: parent_(parent)
, connection_id_(0)
, publisher_xmlrpc_uri_(xmlrpc_uri)
, transport_hints_(transport_hints)
, latched_(false)
{ }

PublisherLink2::~PublisherLink2()
{ }

bool PublisherLink2::setHeader(const Header& header)
{
  header.getValue("callerid", caller_id_);

  std::string md5sum, type, latched_str;
  if (!header.getValue("md5sum", md5sum))
  {
    ROS_ERROR("Publisher header did not have required element: md5sum");
    return false;
  }

  md5sum_ = md5sum;

  if (!header.getValue("type", type))
  {
    ROS_ERROR("Publisher header did not have required element: type");
    return false;
  }

  latched_ = false;
  if (header.getValue("latching", latched_str))
  {
    if (latched_str == "1")
    {
      latched_ = true;
    }
  }

  connection_id_ = ConnectionManager::instance()->getNewConnectionID();
  header_ = header;

  if (SubscriptionPtr2 parent = parent_.lock())
  {
    parent->headerReceived(shared_from_this(), header);
  }

  return true;
}

const std::string& PublisherLink2::getPublisherXMLRPCURI()
{
  return publisher_xmlrpc_uri_;
}

const std::string& PublisherLink2::getMD5Sum()
{
  ROS_ASSERT(!md5sum_.empty());
  return md5sum_;
}

} // namespace ros

