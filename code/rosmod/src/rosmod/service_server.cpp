#include "rosmod/node_handle.h"
#include "rosmod/service_manager.h"

namespace ros
{

ServiceServer2::Impl::Impl() : unadvertised_(false) { }

ServiceServer2::Impl::~Impl()
{
  ROS_DEBUG("ServiceServer on '%s' deregistering callbacks.", service_.c_str());
  unadvertise();
}

bool ServiceServer2::Impl::isValid() const
{
  return !unadvertised_;
}

void ServiceServer2::Impl::unadvertise()
{
  if (!unadvertised_)
  {
    unadvertised_ = true;
    ServiceManager2::instance()->unadvertiseService(service_);
    node_handle_.reset();
  }
}

ServiceServer2::ServiceServer2(const std::string& service, const NodeHandle2& node_handle)
: impl_(new Impl)
{
  impl_->service_ = service;
  impl_->node_handle_ = NodeHandlePtr2(new NodeHandle2(node_handle));
}

ServiceServer2::ServiceServer2(const ServiceServer2& rhs)
{
  impl_ = rhs.impl_;
}

ServiceServer2::~ServiceServer2()
{
}

void ServiceServer2::shutdown()
{
  if (impl_)
  {
    impl_->unadvertise();
  }
}

std::string ServiceServer2::getService() const
{
  if (impl_ && impl_->isValid())
  {
    return impl_->service_;
  }

  return std::string();
}

} // namespace ros
