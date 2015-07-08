#ifndef SENSOR_HPP
#define SENSOR_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "rover_pkg/vessel_state.h"

#include "krpci/krpci.hpp"

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class sensor : public Component
{
public:
  // Constructor
  sensor(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Timer Callback - sensor_timer
  void sensor_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~sensor();

private:

  // Timer
  ros::Timer sensor_timer;

  // Publisher 
  ros::Publisher vessel_state_pub;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
