#ifndef COMPONENT_2_HPP
#define COMPONENT_2_HPP

#include "ros/ros.h"

#include "Component.hpp"

#include "multi_component_example/ComponentService.h"

// --------------------------------
//      USER INCLUDES GO HERE
// --------------------------------

namespace Node1
{
  class Component2 : public Component
  {
  public:
    // These functions' business logic will be filled in by the user:

    // Timer0Callback() generated if model has Timer0
    void Timer0Callback(const ros::TimerEvent& event);
 
    // these functions' business logic will be auto-generated:

    // startUp() is used to configure timers, publishers, & service providers
    void startUp();

    // required for clean shutdown
    ~Component2();

  private:
    ros::Timer Timer0;            // generated if model has Timer0
    ros::ServiceClient compServClient;  // generated if model has compServClient
  };
};

#endif
