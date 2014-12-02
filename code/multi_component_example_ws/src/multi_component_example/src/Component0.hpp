#ifndef COMPONENT_1_HPP
#define COMPONENT_1_HPP

#include "ros/ros.h"

#include "Component.hpp"

#include "multi_component_example/ComponentName.h"
#include "multi_component_example/ComponentService.h"

// --------------------------------
//      USER INCLUDES GO HERE
// --------------------------------

namespace Node1
{
  class Component0 : public Component
  {
  public:
    // These functions' business logic will be filled in by the user:

    // Init() is always generated
    void Init(const ros::TimerEvent& event);

    // OnOneData(compName) generated if model has compNameSub
    void OnOneData(const multi_component_example::ComponentName::ConstPtr& compName);  
    // Timer0Callback() generated if model has Timer0
    void Timer0Callback(const ros::TimerEvent& event);
 
    // these functions' business logic will be auto-generated:

    // startUp() is used to configure timers, publishers, & service providers
    void startUp();

    // required for clean shutdown
    ~Component0();

  private:
    ros::Timer Timer0;            // generated if model has Timer0
    ros::Subscriber compNameSub;  // generated if model has compNameSub
    ros::Publisher compNamePub;   // generated if model has compNamePub
  };
};

#endif
