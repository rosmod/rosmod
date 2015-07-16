#ifndef TIMER_HPP
#define TIMER_HPP

#include "rosmod/rosmod_ros.h"
#include "Component.hpp"
//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Timer : public Component
{
public:
  // Constructor
  Timer(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const rosmod::TimerEvent& event);

  // Timer Callback - periodic_timer
  void periodic_timerCallback(const rosmod::TimerEvent& event);

  // Timer Callback - sporadic_timer
  void sporadic_timerCallback(const rosmod::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Timer();

private:

  // Timer
  rosmod::Timer periodic_timer;

  // Timer
  rosmod::Timer sporadic_timer;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};
#endif
