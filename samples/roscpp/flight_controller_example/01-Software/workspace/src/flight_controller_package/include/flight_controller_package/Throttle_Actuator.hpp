#ifndef THROTTLE_ACTUATOR_HPP
#define THROTTLE_ACTUATOR_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "flight_controller_package/Throttle_Control.h"

#include "krpci/krpci.hpp"

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Throttle_Actuator : public Component
{
public:
  // Constructor
  Throttle_Actuator(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Server Callback - throttle_control_server
  bool Throttle_ControlCallback(flight_controller_package::Throttle_Control::Request &req, 
    flight_controller_package::Throttle_Control::Response &res);

  // Start up
  void startUp();

  // Destructor
  ~Throttle_Actuator();

private:

  // Server 
  ros::ServiceServer throttle_control_server;

  KRPCI krpci_client;
  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
