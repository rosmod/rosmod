#ifndef ACTUATOR_COMPONENT_HPP
#define ACTUATOR_COMPONENT_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "ksp_aeris_controller/Actuation_Command.h"

#include "krpci/krpci.hpp"

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Actuator_Component : public Component
{
public:
  // Constructor
  Actuator_Component(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Subscriber Callback - actuator_control_subscriber
  void actuator_control_subscriber_OnOneData(const ksp_aeris_controller::Actuation_Command::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~Actuator_Component();

private:

  // Subscriber
  ros::Subscriber actuator_control_subscriber; 

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
