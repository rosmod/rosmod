#ifndef ROSMOD_NODE_HANDLE_H
#define ROSMOD_NODE_HANDLE_H

#include "rosmod/timer.h"
#include "rosmod/publisher.h"
#include "rosmod/advertise_service_options.h"
#include "rosmod/advertise_options.h"
#include "rosmod/service_server.h"
#include "rosmod/subscriber.h"
#include "rosmod/subscribe_options.h"
#include "rosmod/cmq.h"
#include "rosmod/service_manager.h"
#include "rosmod/topic_manager.h"
#include "ros/node_handle.h"

namespace ros
{

class ServiceServer2;
class Subscriber2;
class NodeHandleBackingCollection2;

class NodeHandle2 : public NodeHandle
{
  public:

    NodeHandle2();
    ServiceServer2 advertiseService(AdvertiseServiceOptions2& ops);
    Subscriber2 subscribe(SubscribeOptions2& ops);

    Publisher2 advertise(AdvertiseOptions2& ops);

    template <class M>
    Publisher2 advertise(const std::string& topic, uint32_t queue_size, bool latch = false)
    {
      AdvertiseOptions2 ops;
      ops.template init<M>(topic, queue_size);
      ops.latch = latch;
      return advertise(ops);
    }

    /**
    * \brief Create a timer which will call a callback at the specified rate.  This variant allows
    * the full range of TimerOptions. 
    *
    * When the Timer (and all copies of it) returned goes out of scope, the timer will automatically
    * be stopped, and the callback will no longer be called. 
    *
    * \param ops The options to use when creating the timer
    */
    Timer2 createTimer(TimerOptions2& ops) const;

    CMQ* callback_queue_;

  private:
    NodeHandleBackingCollection2* collection_;

};

typedef boost::shared_ptr<NodeHandle2> NodeHandlePtr2;

/**
 * \brief Manages an service advertisement.
 *
 * A ServiceServer should always be created through a call to NodeHandle::advertiseService(), or copied from
 * one that was.  Once all copies of a specific
 * ServiceServer go out of scope, the service associated with it will be unadvertised and the service callback
 * will stop being called.
 */
class ServiceServer2
{
public:
  ServiceServer2() {}
  ServiceServer2(const ServiceServer2& rhs);
  ~ServiceServer2();

  /**
   * \brief Unadvertise the service associated with this ServiceServer
   *
   * This method usually does not need to be explicitly called, as automatic shutdown happens when
   * all copies of this ServiceServer go out of scope
   *
   * This method overrides the automatic reference counted unadvertise, and immediately
   * unadvertises the service associated with this ServiceServer
   */
  void shutdown();

  std::string getService() const;

  operator void*() const { return (impl_ && impl_->isValid()) ? (void*)1 : (void*)0; }

  bool operator<(const ServiceServer2& rhs) const
  {
    return impl_ < rhs.impl_;
  }

  bool operator==(const ServiceServer2& rhs) const
  {
    return impl_ == rhs.impl_;
  }

  bool operator!=(const ServiceServer2& rhs) const
  {
    return impl_ != rhs.impl_;
  }

private:

  class Impl
  {
  public:
    Impl();
    ~Impl();

    void unadvertise();
    bool isValid() const;

    std::string service_;
    NodeHandlePtr2 node_handle_;
    bool unadvertised_;
  };
  typedef boost::shared_ptr<Impl> ImplPtr;
  typedef boost::weak_ptr<Impl> ImplWPtr;

  ImplPtr impl_;

  ServiceServer2(const std::string& service, const NodeHandle2& node_handle);

  friend class NodeHandle2;
  friend class NodeHandleBackingCollection2;
};

typedef std::vector<ServiceServer2> V_ServiceServer2;

}

#endif
