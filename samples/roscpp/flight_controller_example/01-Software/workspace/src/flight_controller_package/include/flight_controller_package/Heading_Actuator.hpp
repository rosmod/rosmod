#ifndef HEADING_ACTUATOR_HPP
#define HEADING_ACTUATOR_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "flight_controller_package/Heading_Control.h"

#include "krpci/krpci.hpp"

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Heading_Actuator : public Component
{
public:
  // Constructor
  Heading_Actuator(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Server Callback - heading_control_server
  bool Heading_ControlCallback(flight_controller_package::Heading_Control::Request &req, 
    flight_controller_package::Heading_Control::Response &res);

  // Start up
  void startUp();

  // Destructor
  ~Heading_Actuator();

private:

  // Server 
  ros::ServiceServer heading_control_server;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
