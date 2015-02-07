#include "rosmod/subscriber.h"
#include "rosmod/topic_manager.h"
#include "rosmod/advertise_options.h"
#include "rosmod/node_handle.h"

namespace ros
{

  Subscriber2::Impl::Impl()
  : unsubscribed_(false)
  { }

  Subscriber2::Impl::~Impl()
  {
    ROS_DEBUG("Subscriber on '%s' deregistering callbacks.", topic_.c_str());
    unsubscribe();
  }

  bool Subscriber2::Impl::isValid() const
  {
    return !unsubscribed_;
  }

  void Subscriber2::Impl::unsubscribe()
  {
    if (!unsubscribed_)
      {
	unsubscribed_ = true;
	TopicManager2::instance()->unsubscribe(topic_, helper_);
	node_handle_.reset();
	helper_.reset();
      }
  }

  Subscriber2::Subscriber2(const std::string& topic, const NodeHandle2& node_handle, 
			 const SubscriptionCallbackHelperPtr& helper)
  : impl_(new Impl)
  {
    impl_->topic_ = topic;
    impl_->node_handle_ = NodeHandlePtr2(new NodeHandle2(node_handle));
    impl_->helper_ = helper;
  }

  Subscriber2::Subscriber2(const Subscriber2& rhs)
  {
    impl_ = rhs.impl_;
  }

  Subscriber2::~Subscriber2()
  {
  }

  void Subscriber2::shutdown()
  {
    if (impl_)
      {
	impl_->unsubscribe();
      }
  }

  std::string Subscriber2::getTopic() const
  {
    if (impl_)
      {
	return impl_->topic_;
      }

    return std::string();
  }

  uint32_t Subscriber2::getNumPublishers() const
  {
    if (impl_ && impl_->isValid())
      {
	return TopicManager2::instance()->getNumPublishers(impl_->topic_);
      }

    return 0;
  }

} // namespace ros
