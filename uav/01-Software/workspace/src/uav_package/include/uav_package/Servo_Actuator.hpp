#ifndef SERVO_ACTUATOR_HPP
#define SERVO_ACTUATOR_HPP
#include "node/Component.hpp"
#include "uav_package/control_command.h"

#ifdef USE_ROSMOD
  #include "rosmod/rosmod_ros.h"
#else
  #ifdef USE_ROSCPP
    #include "ros/ros.h"
  #endif
#endif



//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Servo_Actuator : public Component
{
public:
  // Constructor
  Servo_Actuator(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - control_command_subscriber
  void control_command_subscriber_operation(const uav_package::control_command::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~Servo_Actuator();

private:

  // Subscriber
  NAMESPACE::Subscriber control_command_subscriber;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
