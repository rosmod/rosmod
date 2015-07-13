#ifndef HEADING_SENSOR_HPP
#define HEADING_SENSOR_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "flight_controller_package/Heading.h"

#include "krpci/krpci.hpp"

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Heading_Sensor : public Component
{
public:
  // Constructor
  Heading_Sensor(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Timer Callback - heading_sensor_timer
  void heading_sensor_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Heading_Sensor();

private:

  // Timer
  ros::Timer heading_sensor_timer;

  // Publisher 
  ros::Publisher heading_publisher;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
