#include "rosmod/subscriber_link.h"
#include "rosmod/publication.h"

#include <boost/bind.hpp>

namespace ros
{

SubscriberLink2::SubscriberLink2()
  : connection_id_(0)
{

}

SubscriberLink2::~SubscriberLink2()
{

}

bool SubscriberLink2::verifyDatatype(const std::string &datatype)
{
  PublicationPtr2 parent = parent_.lock();
  if (!parent)
  {
    ROS_ERROR("Trying to verify the datatype on a publisher without a parent");
    ROS_BREAK();

    return false;
  }

  if (datatype != parent->getDataType())
  {
    ROS_ERROR( "tried to send a message with type %s on a " \
                       "TransportSubscriberLink that has datatype %s",
                datatype.c_str(), parent->getDataType().c_str());
    return false; // todo: figure out a way to log this error
  }

  return true;
}

const std::string& SubscriberLink2::getMD5Sum()
{
  PublicationPtr2 parent = parent_.lock();
  return parent->getMD5Sum();
}

const std::string& SubscriberLink2::getDataType()
{
  PublicationPtr2 parent = parent_.lock();
  return parent->getDataType();
}

const std::string& SubscriberLink2::getMessageDefinition()
{
  PublicationPtr2 parent = parent_.lock();
  return parent->getMessageDefinition();
}

}
