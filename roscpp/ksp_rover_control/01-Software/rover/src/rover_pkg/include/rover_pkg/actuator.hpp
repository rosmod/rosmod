#ifndef ACTUATOR_HPP
#define ACTUATOR_HPP
#include "ros/ros.h"
#include "node/Component.hpp"
#include "rover_pkg/control_command.h"

#include "krpci/krpci.hpp"


//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class actuator : public Component
{
public:
  // Constructor
  actuator(ComponentConfig& _config, int argc, char **argv) : Component(_config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Subscriber Callback - control_command_sub
  void control_command_sub_OnOneData(const rover_pkg::control_command::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~actuator();

private:

  // Subscriber
  ros::Subscriber control_command_sub;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
