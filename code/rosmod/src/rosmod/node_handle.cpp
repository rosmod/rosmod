#include "rosmod/node_handle.h"

namespace ros
{

struct AdvertiseOptions;

class NodeHandleBackingCollection2
{
public:
  typedef std::vector<Publisher2::ImplWPtr> V_PubImpl;
  typedef std::vector<ServiceServer2::ImplWPtr> V_SrvImpl;
  typedef std::vector<Subscriber2::ImplWPtr> V_SubImpl;
  //typedef std::vector<ServiceClient::ImplWPtr> V_SrvCImpl;
  V_PubImpl pubs_;
  V_SrvImpl srvs_;
  V_SubImpl subs_;
  //V_SrvCImpl srv_cs_;

  boost::mutex mutex_;
};

NodeHandle2::NodeHandle2()
{
  collection_ = new NodeHandleBackingCollection2;
}

ServiceServer2 NodeHandle2::advertiseService(AdvertiseServiceOptions2& ops)
{
  ops.service = resolveName(ops.service);
  if (ops.callback_queue == 0)
  {
    if (callback_queue_)
    {
      ops.callback_queue = callback_queue_;
    }
    else
    {
      ops.callback_queue = (ros::CMQ *)getGlobalCallbackQueue();
    }
  }

  if (ServiceManager2::instance()->advertiseService(ops))
  {
    ServiceServer2 srv(ops.service, *this);

    {
      boost::mutex::scoped_lock lock(collection_->mutex_);
      collection_->srvs_.push_back(srv.impl_);
    }
    return srv;
  }
  return ServiceServer2();
}

Publisher2 NodeHandle2::advertise(AdvertiseOptions2& ops)
{
  ROS_INFO("Advertising...");
  ops.topic = resolveName(ops.topic);
  if (ops.callback_queue == 0)
  {
    if (callback_queue_)
    {
      ops.callback_queue = callback_queue_;
    }
    else
    {
      ops.callback_queue = (ros::CMQ *)getGlobalCallbackQueue();
    }
  }
  ROS_INFO("Creating Subscriber Callbacks Ptr2...");
  SubscriberCallbacksPtr2 callbacks(new SubscriberCallbacks2(ops.connect_cb, ops.disconnect_cb, 
							   ops.tracked_object, ops.callback_queue));
  ROS_INFO("Done Creating Subscriber Callbacks Ptr2...");
  if (TopicManager2::instance()->advertise(ops, callbacks))
  {
    ROS_INFO("Creating Publisher2");
    Publisher2 pub(ops.topic, ops.md5sum, ops.datatype, *this, callbacks);
    ROS_INFO("Done Creating Publisher2");
    {
      boost::mutex::scoped_lock lock(collection_->mutex_);
      ROS_INFO("About to Collection Pushback");
      collection_->pubs_.push_back(pub.impl_);
      ROS_INFO("Done Collection Pushback");
    }

    return pub;
  }
  ROS_INFO("Returning...");
  return Publisher2();
}

Subscriber2 NodeHandle2::subscribe(SubscribeOptions2& ops)
{
  ops.topic = resolveName(ops.topic);
  if (ops.callback_queue == 0)
  {
    if (callback_queue_)
    {
      ops.callback_queue = callback_queue_;
    }
    else
    {
      ops.callback_queue = (ros::CMQ *)getGlobalCallbackQueue();
    }
  }

  if (TopicManager2::instance()->subscribe(ops))
  {
    Subscriber2 sub(ops.topic, *this, ops.helper);
    {
      boost::mutex::scoped_lock lock(collection_->mutex_);
      if (collection_)
          collection_->subs_.push_back(sub.impl_);
      else {
          collection_ = new NodeHandleBackingCollection2;
          collection_->subs_.push_back(sub.impl_);
      }
    }

    return sub;
  }

  return Subscriber2();
}

Timer2 NodeHandle2::createTimer(TimerOptions2& ops) const
{
  if (ops.callback_queue == 0)
  {
    if (callback_queue_)
    {
      ops.callback_queue = callback_queue_;
    }
    else
    {
      ops.callback_queue = (ros::CMQ *)getGlobalCallbackQueue();
    }
  }

  Timer2 timer(ops);
  if (ops.autostart)
    timer.start();
  return timer;
}

}
