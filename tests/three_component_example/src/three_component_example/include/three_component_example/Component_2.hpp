#ifndef COMPONENT_2_HPP
#define COMPONENT_2_HPP
#include "node/Component.hpp"
#include "three_component_example/ComponentName.h"
#include "three_component_example/ComponentService.h"

#ifdef USE_ROSMOD
  #include "rosmod/rosmod_ros.h"
#else
  #ifdef USE_ROSCPP
    #include "ros/ros.h"
  #endif
#endif

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Component_2 : public Component
{
public:
  // Constructor
  Component_2(ComponentConfig& _config, int argc, char **argv) : Component(_config, argc, argv) {}

  // Initialization
  void Init(const NAMESPACE::TimerEvent& event);

  // Server Callback - Service_Server
  bool ComponentServiceCallback(three_component_example::ComponentService::Request &req, 
    three_component_example::ComponentService::Response &res);

  // Timer Callback - Timer_2
  void Timer_2Callback(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Component_2();

private:

  // Timer
  NAMESPACE::Timer Timer_2;

  // Publisher 
  NAMESPACE::Publisher Name_Publisher;

  // Server 
  NAMESPACE::ServiceServer Service_Server;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
