#ifndef SENSOR_COMPONENT_HPP
#define SENSOR_COMPONENT_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "ksp_stearwing_controller/Sensor_Reading.h"

#include "krpci/krpci.hpp"

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Sensor_Component : public Component
{
public:
  // Constructor
  Sensor_Component(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Timer Callback - sensor_timer
  void sensor_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Sensor_Component();

private:

  // Timer
  ros::Timer sensor_timer;

  // Publisher 
  ros::Publisher sensor_publisher;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
