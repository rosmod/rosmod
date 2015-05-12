#ifndef COMPONENT_2_HPP
#define COMPONENT_2_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "three_component_example/ComponentName.h"
#include "three_component_example/ComponentService.h"
//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Component_2 : public Component
{
public:
  // Constructor
  Component_2(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Server Callback - Service_Server
  bool ComponentServiceCallback(three_component_example::ComponentService::Request &req, 
    three_component_example::ComponentService::Response &res);

  // Timer Callback - Timer_2
  void Timer_2Callback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Component_2();

private:

  // Timer
  ros::Timer Timer_2;

  // Publisher 
  ros::Publisher Name_Publisher;

  // Server 
  ros::ServiceServer Service_Server;

  //# Start User Private Variables Marker
                  
  //# End User Private Variables Marker
};


#endif
