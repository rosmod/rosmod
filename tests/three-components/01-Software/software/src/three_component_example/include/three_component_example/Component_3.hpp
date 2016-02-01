#ifndef COMPONENT_3_HPP
#define COMPONENT_3_HPP
#include "node/Component.hpp"
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

class Component_3 : public Component
{
public:
  // Constructor
  Component_3(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Timer Operation - Timer_3
  void Timer_3_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Component_3();

private:

  // Timer
  NAMESPACE::Timer Timer_3;

  // Client 
  NAMESPACE::ServiceClient Service_Client;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
