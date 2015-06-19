#ifndef LONGITUDE_SENSOR_HPP
#define LONGITUDE_SENSOR_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "flight_controller_package/Longitude.h"


//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Longitude_Sensor : public Component
{
public:
  // Constructor
  Longitude_Sensor(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Timer Callback - longitude_sensor_timer
  void longitude_sensor_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Longitude_Sensor();

private:

  // Timer
  ros::Timer longitude_sensor_timer;

  // Publisher 
  ros::Publisher longitude_publisher;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
