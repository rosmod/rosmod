#ifndef PITCH_SENSOR_HPP
#define PITCH_SENSOR_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "flight_controller_package/Pitch.h"

#include "krpci/krpci.hpp"

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Pitch_Sensor : public Component
{
public:
  // Constructor
  Pitch_Sensor(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Timer Callback - pitch_sensor_timer
  void pitch_sensor_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Pitch_Sensor();

private:

  // Timer
  ros::Timer pitch_sensor_timer;

  // Publisher 
  ros::Publisher pitch_publisher;

  KRPCI krpci_client("Pitch_Sensor_krpci");
  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
