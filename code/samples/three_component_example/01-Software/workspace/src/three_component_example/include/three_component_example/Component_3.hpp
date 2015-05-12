#ifndef COMPONENT_3_HPP
#define COMPONENT_3_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "three_component_example/ComponentService.h"
//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Component_3 : public Component
{
public:
  // Constructor
  Component_3(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Timer Callback - Timer_3
  void Timer_3Callback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Component_3();

private:

  // Timer
  ros::Timer Timer_3;

  // Client 
  ros::ServiceClient Service_Client;

  //# Start User Private Variables Marker
                  
  //# End User Private Variables Marker
};


#endif
