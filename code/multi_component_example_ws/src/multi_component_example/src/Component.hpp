#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "ros/ros.h"

// required for the callback queue
#include <ros/callback_queue.h>

class Component
{
public:
  // These functions must be overridden
  // Init will be generated with BL supplied by user
  virtual void Init(const ros::TimerEvent& event) = 0;
  // StartUp will be completely generated
  virtual void startUp() = 0;

  // queueThread processes queue actions
  void processQueue();

  // required for clean shutdown
  ~Component();

protected:
  ros::Timer initOneShotTimer;  // timer for calling init
  ros::CallbackQueue compQueue; // single callbackQueue for the component
};

#endif
