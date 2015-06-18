#ifndef YAW_ACTUATOR_HPP
#define YAW_ACTUATOR_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "flight_controller_package/Yaw_Control.h"

#include "krpci/krpci.hpp"

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Yaw_Actuator : public Component
{
public:
  // Constructor
  Yaw_Actuator(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Server Callback - yaw_control_server
  bool Yaw_ControlCallback(flight_controller_package::Yaw_Control::Request &req, 
    flight_controller_package::Yaw_Control::Response &res);

  // Start up
  void startUp();

  // Destructor
  ~Yaw_Actuator();

private:

  // Server 
  ros::ServiceServer yaw_control_server;

  KRPCI krpci_client;
  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
