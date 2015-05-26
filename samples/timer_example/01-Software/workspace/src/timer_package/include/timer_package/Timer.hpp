#ifndef TIMER_HPP
#define TIMER_HPP

#include "ros/ros.h"
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
  void Init(const ros::TimerEvent& event);

  // Timer Callback - periodic_timer
  void periodic_timerCallback(const ros::TimerEvent& event);

  // Timer Callback - sporadic_timer
  void sporadic_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Timer();

private:

  // Timer
  ros::Timer periodic_timer;

  // Timer
  ros::Timer sporadic_timer;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};


#endif
