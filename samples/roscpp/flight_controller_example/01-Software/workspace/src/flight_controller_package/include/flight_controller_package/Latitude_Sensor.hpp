#ifndef LATITUDE_SENSOR_HPP
#define LATITUDE_SENSOR_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "flight_controller_package/Latitude.h"


//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Latitude_Sensor : public Component
{
public:
  // Constructor
  Latitude_Sensor(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Timer Callback - latitude_sensor_timer
  void latitude_sensor_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Latitude_Sensor();

private:

  // Timer
  ros::Timer latitude_sensor_timer;

  // Publisher 
  ros::Publisher latitude_publisher;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
