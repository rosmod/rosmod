#ifndef COMPONENT_5_HPP
#define COMPONENT_5_HPP
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

class Component_5 : public Component
{
public:
  // Constructor
  Component_5(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Timer Operation - Timer_5
  void Timer_5_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Component_5();

private:

  // Timer
  NAMESPACE::Timer Timer_5;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
