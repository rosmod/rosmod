// #include "rosmod/publisher.h"
#include "rosmod/publication.h"
#include "rosmod/topic_manager.h"
#include "rosmod/node_handle.h"

namespace ros
{

Publisher2::Impl::Impl() : unadvertised_(false) { }

Publisher2::Impl::~Impl()
{
  ROS_DEBUG("Publisher on '%s' deregistering callbacks.", topic_.c_str());
  unadvertise();
}

bool Publisher2::Impl::isValid() const
{
  return !unadvertised_;
}

void Publisher2::Impl::unadvertise()
{
  if (!unadvertised_)
  {
    unadvertised_ = true;
    TopicManager2::instance()->unadvertise(topic_, callbacks_);
    node_handle_.reset();
  }
}

Publisher2::Publisher2(const std::string& topic, const std::string& md5sum, const std::string& datatype, const NodeHandle2& node_handle, const SubscriberCallbacksPtr2& callbacks)
: impl_(new Impl)
{
  impl_->topic_ = topic;
  impl_->md5sum_ = md5sum;
  impl_->datatype_ = datatype;
  impl_->node_handle_ = NodeHandlePtr2(new NodeHandle2(node_handle));
  impl_->callbacks_ = callbacks;
}

Publisher2::Publisher2(const Publisher2& rhs)
{
  impl_ = rhs.impl_;
}

Publisher2::~Publisher2()
{
}

void Publisher2::publish(const boost::function<SerializedMessage(void)>& serfunc, SerializedMessage& m) const
{
  if (!impl_)
  {
    ROS_ASSERT_MSG(false, "Call to publish() on an invalid Publisher (topic [%s])", impl_->topic_.c_str());
    return;
  }

  if (!impl_->isValid())
  {
    ROS_ASSERT_MSG(false, "Call to publish() on an invalid Publisher (topic [%s])", impl_->topic_.c_str());
    return;
  }

  TopicManager2::instance()->publish(impl_->topic_, serfunc, m);
}

void Publisher2::incrementSequence() const
{
  if (impl_ && impl_->isValid())
  {
    TopicManager2::instance()->incrementSequence(impl_->topic_);
  }
}

void Publisher2::shutdown()
{
  if (impl_)
  {
    impl_->unadvertise();
    impl_.reset();
  }
}

std::string Publisher2::getTopic() const
{
  if (impl_)
  {
    return impl_->topic_;
  }

  return std::string();
}

uint32_t Publisher2::getNumSubscribers() const
{
  if (impl_ && impl_->isValid())
  {
    return TopicManager2::instance()->getNumSubscribers(impl_->topic_);
  }

  return 0;
}

bool Publisher2::isLatched() const {
  PublicationPtr2 publication_ptr;
  if (impl_ && impl_->isValid()) {
    publication_ptr =
      TopicManager2::instance()->lookupPublication(impl_->topic_);
  } else {
    ROS_ASSERT_MSG(false, "Call to isLatched() on an invalid Publisher");
    throw ros::Exception("Call to isLatched() on an invalid Publisher");
  }
  if (publication_ptr) {
    return publication_ptr->isLatched();
  } else {
    ROS_ASSERT_MSG(false, "Call to isLatched() on an invalid Publisher");
    throw ros::Exception("Call to isLatched() on an invalid Publisher");
  }
}

} // namespace ros
