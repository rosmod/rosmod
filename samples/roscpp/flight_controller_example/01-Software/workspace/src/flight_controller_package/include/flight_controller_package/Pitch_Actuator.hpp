#ifndef PITCH_ACTUATOR_HPP
#define PITCH_ACTUATOR_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "flight_controller_package/Pitch_Control.h"

#include "krpci/krpci.hpp"

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Pitch_Actuator : public Component
{
public:
  // Constructor
  Pitch_Actuator(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Server Callback - pitch_control_server
  bool Pitch_ControlCallback(flight_controller_package::Pitch_Control::Request &req, 
    flight_controller_package::Pitch_Control::Response &res);

  // Start up
  void startUp();

  // Destructor
  ~Pitch_Actuator();

private:

  // Server 
  ros::ServiceServer pitch_control_server;

  KRPCI krpci_client("Pitch_Actuator_krpci");
  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
