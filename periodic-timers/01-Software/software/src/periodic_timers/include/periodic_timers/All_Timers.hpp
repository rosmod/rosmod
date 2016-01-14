#ifndef ALL_TIMERS_HPP
#define ALL_TIMERS_HPP
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

class All_Timers : public Component
{
public:
  // Constructor
  All_Timers(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Timer Operation - Timer_1
  void Timer_1_operation(const NAMESPACE::TimerEvent& event);

  // Timer Operation - Timer_2
  void Timer_2_operation(const NAMESPACE::TimerEvent& event);

  // Timer Operation - Timer_3
  void Timer_3_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~All_Timers();

private:

  // Timer
  NAMESPACE::Timer Timer_1;

  // Timer
  NAMESPACE::Timer Timer_2;

  // Timer
  NAMESPACE::Timer Timer_3;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
