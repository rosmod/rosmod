#ifndef TIMER_HPP
#define TIMER_HPP
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

class Timer : public Component
{
public:
  // Constructor
  Timer(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Timer Operation - periodic_timer
  void periodic_timer_operation(const NAMESPACE::TimerEvent& event);

  // Timer Operation - sporadic_timer
  void sporadic_timer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Timer();

private:

  // Timer
  NAMESPACE::Timer periodic_timer;

  // Timer
  NAMESPACE::Timer sporadic_timer;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
