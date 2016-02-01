#ifndef COMPONENT_4_HPP
#define COMPONENT_4_HPP
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

class Component_4 : public Component
{
public:
  // Constructor
  Component_4(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Timer Operation - Timer_4
  void Timer_4_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Component_4();

private:

  // Timer
  NAMESPACE::Timer Timer_4;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
