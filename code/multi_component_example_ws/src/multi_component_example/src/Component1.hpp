#ifndef COMPONENT_1_HPP
#define COMPONENT_1_HPP

#include "ros/ros.h"

#include "Component.hpp"

#include "multi_component_example/ComponentName.h"

// --------------------------------
//      USER INCLUDES GO HERE
// --------------------------------

namespace Node1
{
  class Component1 : public Component
  {
  public:
    // These functions' business logic will be filled in by the user:

    // Timer0Callback() generated if model has Timer0
    void Timer0Callback(const ros::TimerEvent& event);
 
    // these functions' business logic will be auto-generated:

    // startUp() is used to configure timers, publishers, & service providers
    void startUp();

    // required for clean shutdown
    ~Component1();

  private:
    ros::Timer Timer0;            // generated if model has Timer0
    ros::Publisher compNamePub;   // generated if model has compNamePub
  };
};

#endif
