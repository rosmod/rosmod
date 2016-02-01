#ifndef COMPONENT_1_HPP
#define COMPONENT_1_HPP
#include "node/Component.hpp"

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

class Component_1 : public Component
{
public:
  // Constructor
  Component_1(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Timer Operation - Timer_1
  void Timer_1_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Component_1();

private:

  // Timer
  NAMESPACE::Timer Timer_1;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
