#ifndef COMPONENT_2_HPP
#define COMPONENT_2_HPP
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

class Component_2 : public Component
{
public:
  // Constructor
  Component_2(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Timer Operation - Timer_2
  void Timer_2_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Component_2();

private:

  // Timer
  NAMESPACE::Timer Timer_2;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
