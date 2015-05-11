#ifndef COMPONENT_1_HPP
#define COMPONENT_1_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "three_component_example/ComponentName.h"
#include "three_component_example/ComponentName.h"
//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Component_1 : public Component
{
public:
  // Constructor
  Component_1(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Subscriber Callback - Name_Subscriber
  void Name_Subscriber_OnOneData(const three_component_example::ComponentName::ConstPtr& received_data); 
 
  // Timer Callback - Timer_1
  void Timer_1Callback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Component_1();

private:

  // Timer
  ros::Timer Timer_1;

  // Subscriber
  ros::Subscriber Name_Subscriber; 

  // Publisher 
  ros::Publisher Name_Publisher;

  //# Start User Private Variables Marker
    
  //# End User Private Variables Marker
};


#endif
