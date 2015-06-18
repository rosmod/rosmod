#ifndef ROLL_ACTUATOR_HPP
#define ROLL_ACTUATOR_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "flight_controller_package/Roll_Control.h"

#include "krpci/krpci.hpp"

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Roll_Actuator : public Component
{
public:
  // Constructor
  Roll_Actuator(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Server Callback - roll_control_server
  bool Roll_ControlCallback(flight_controller_package::Roll_Control::Request &req, 
    flight_controller_package::Roll_Control::Response &res);

  // Start up
  void startUp();

  // Destructor
  ~Roll_Actuator();

private:

  // Server 
  ros::ServiceServer roll_control_server;

  KRPCI krpci_client("Roll_Actuator_krpci");
  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
