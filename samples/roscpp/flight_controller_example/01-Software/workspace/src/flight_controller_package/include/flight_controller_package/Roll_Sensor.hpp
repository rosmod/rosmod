#ifndef ROLL_SENSOR_HPP
#define ROLL_SENSOR_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "flight_controller_package/Pitch.h"

#include "krpci/krpci.hpp"

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Roll_Sensor : public Component
{
public:
  // Constructor
  Roll_Sensor(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Timer Callback - roll_sensor_timer
  void roll_sensor_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Roll_Sensor();

private:

  // Timer
  ros::Timer roll_sensor_timer;

  // Publisher 
  ros::Publisher roll_publisher;

  KRPCI krpci_client;
  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
