#ifndef VERTICAL_ACTUATOR_CONTROLLER_HPP
#define VERTICAL_ACTUATOR_CONTROLLER_HPP
#include "node/Component.hpp"
#include "agse_package/controlInputs.h"
#include "agse_package/verticalPos.h"

#ifdef USE_ROSMOD
  #include "rosmod/rosmod_ros.h"
#else
  #ifdef USE_ROSCPP
    #include "ros/ros.h"
  #endif
#endif

//# Start User Includes Marker
#include "agse_package/gpio.h"
#include "agse_package/eqep.h"
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class vertical_actuator_controller : public Component
{
public:
  // Constructor
  vertical_actuator_controller(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - controlInputs_sub
  void controlInputs_sub_operation(const agse_package::controlInputs::ConstPtr& received_data); 
 
  // Server Operation - verticalPos_server
  bool verticalPos_operation(agse_package::verticalPos::Request &req, 
    agse_package::verticalPos::Response &res);

  // Timer Operation - verticalPosTimer
  void verticalPosTimer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~vertical_actuator_controller();

private:

  // Timer
  NAMESPACE::Timer verticalPosTimer;

  // Subscriber
  NAMESPACE::Subscriber controlInputs_sub;

  // Server 
  NAMESPACE::ServiceServer verticalPos_server;

  //# Start User Private Variables Marker
  bool paused;
  // epsion value for minimum actionable difference between goal and current
  int epsilon;
  // goal position for the vertical linear actuator
  int verticalGoal;
  // current position of the vertical linear actuator
  int verticalCurrent;
  // pin that motor forward is connected to
  unsigned int motorForwardPin;
  // pin that motor backward is connected to
  unsigned int motorBackwardPin;
  // pin that lower limit switch is connected to
  unsigned int lowerLimitSwitchPin;
  // state variable to keep track of whether we've reached the limit or not
  bool lowerLimitReached;
  // ADC the motor potentiometer is connected to (for the prototype)
  int adcPin;
  // enhanced Quadrature Encoder Pulse eQEP module for the vertical actuator
  eQEP verticalMotoreQEP;
  long vm_eqep_period;
  //# End User Private Variables Marker
};

#endif
