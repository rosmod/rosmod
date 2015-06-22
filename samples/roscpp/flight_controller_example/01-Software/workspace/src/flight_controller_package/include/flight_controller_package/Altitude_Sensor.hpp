#ifndef ALTITUDE_SENSOR_HPP
#define ALTITUDE_SENSOR_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "flight_controller_package/Altitude.h"

#include "krpci/krpci.hpp"

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Altitude_Sensor : public Component
{
public:
  // Constructor
  Altitude_Sensor(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Timer Callback - altitude_sensor_timer
  void altitude_sensor_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Altitude_Sensor();

private:

  // Timer
  ros::Timer altitude_sensor_timer;

  // Publisher 
  ros::Publisher altitude_publisher;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
