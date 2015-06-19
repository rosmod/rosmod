#ifndef SPEED_SENSOR_HPP
#define SPEED_SENSOR_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "flight_controller_package/Speed.h"

#include "krpci/krpci.hpp"

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Speed_Sensor : public Component
{
public:
  // Constructor
  Speed_Sensor(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Timer Callback - speed_sensor_timer
  void speed_sensor_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Speed_Sensor();

private:

  // Timer
  ros::Timer speed_sensor_timer;

  // Publisher 
  ros::Publisher speed_publisher;

  KRPCI krpci_client;
  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
