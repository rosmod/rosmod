#include "rosmod/single_subscriber_publisher.h"
#include "rosmod/subscriber_link.h"

namespace ros
{
SingleSubscriberPublisher2::SingleSubscriberPublisher2(const SubscriberLinkPtr2& link)
: link_(link)
{
}

SingleSubscriberPublisher2::~SingleSubscriberPublisher2()
{
}

void SingleSubscriberPublisher2::publish(const SerializedMessage& m) const
{
  link_->enqueueMessage(m, true, true);
}

std::string SingleSubscriberPublisher2::getTopic() const
{
  return link_->getTopic();
}

std::string SingleSubscriberPublisher2::getSubscriberName() const
{
  return link_->getDestinationCallerID();
}

}
