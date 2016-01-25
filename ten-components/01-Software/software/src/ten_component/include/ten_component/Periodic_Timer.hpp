#ifndef PERIODIC_TIMER_HPP
#define PERIODIC_TIMER_HPP
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

class Periodic_Timer : public Component
{
public:
  // Constructor
  Periodic_Timer(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Timer Operation - one_sec_timer
  void one_sec_timer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Periodic_Timer();

private:

  // Timer
  NAMESPACE::Timer one_sec_timer;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
