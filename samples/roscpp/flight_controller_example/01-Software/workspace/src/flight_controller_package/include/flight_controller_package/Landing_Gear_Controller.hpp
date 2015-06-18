#ifndef LANDING_GEAR_CONTROLLER_HPP
#define LANDING_GEAR_CONTROLLER_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "flight_controller_package/Landing_Gear_Control.h"

#include "krpci/krpci.hpp"

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Landing_Gear_Controller : public Component
{
public:
  // Constructor
  Landing_Gear_Controller(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Server Callback - landing_gear_control_server
  bool Landing_Gear_ControlCallback(flight_controller_package::Landing_Gear_Control::Request &req, 
    flight_controller_package::Landing_Gear_Control::Response &res);

  // Start up
  void startUp();

  // Destructor
  ~Landing_Gear_Controller();

private:

  // Server 
  ros::ServiceServer landing_gear_control_server;

  KRPCI krpci_client("Landing_Gear_Controller_krpci");
  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
