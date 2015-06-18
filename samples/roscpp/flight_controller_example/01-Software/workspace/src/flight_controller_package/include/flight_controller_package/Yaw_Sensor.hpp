#ifndef YAW_SENSOR_HPP
#define YAW_SENSOR_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "flight_controller_package/Pitch.h"

#include "krpci/krpci.hpp"

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Yaw_Sensor : public Component
{
public:
  // Constructor
  Yaw_Sensor(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Timer Callback - yaw_sensor_timer
  void yaw_sensor_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Yaw_Sensor();

private:

  // Timer
  ros::Timer yaw_sensor_timer;

  // Publisher 
  ros::Publisher yaw_publisher;

  KRPCI krpci_client;
  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
