#ifndef ACTUATOR_HPP
#define ACTUATOR_HPP
#include "node/Component.hpp"
#include "rover_pkg/control_command.h"

#ifdef USE_ROSMOD
  #include "rosmod/rosmod_ros.h"
#else
  #ifdef USE_ROSCPP
    #include "ros/ros.h"
  #endif
#endif

#include "krpci/krpci.hpp"


//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class actuator : public Component
{
public:
  // Constructor
  actuator(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - control_command_sub
  void control_command_sub_operation(const rover_pkg::control_command::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~actuator();

private:

  // Subscriber
  NAMESPACE::Subscriber control_command_sub;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
