#ifndef RADIAL_ACTUATOR_CONTROLLER_HPP
#define RADIAL_ACTUATOR_CONTROLLER_HPP
#include "node/Component.hpp"
#include "agse_package/controlInputs.h"
#include "agse_package/radialPos.h"

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

class radial_actuator_controller : public Component
{
public:
  // Constructor
  radial_actuator_controller(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - controlInputs_sub
  void controlInputs_sub_operation(const agse_package::controlInputs::ConstPtr& received_data); 
 
  // Server Operation - radialPos_server
  bool radialPos_operation(agse_package::radialPos::Request &req, 
    agse_package::radialPos::Response &res);

  // Timer Operation - radialPosTimer
  void radialPosTimer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~radial_actuator_controller();

private:

  // Timer
  NAMESPACE::Timer radialPosTimer;

  // Subscriber
  NAMESPACE::Subscriber controlInputs_sub;

  // Server 
  NAMESPACE::ServiceServer radialPos_server;

  //# Start User Private Variables Marker
  // paused variable which is controlled by the pause switch
  bool paused;
  // epsion value for minimum actionable difference between goal and current
  int epsilon;
  // goal position for the radial linear actuator
  int radialGoal;
  // current position of the radial linear actuator
  int radialCurrent;
  // pin that motor forward is connected to
  unsigned int motorForwardPin;
  // pin that motor backward is connected to
  unsigned int motorBackwardPin;
  // pin that lowerLimitSwitch is connected to
  unsigned int lowerLimitSwitchPin;
  // state variable to keep track of whether we've reached the limit or not
  bool lowerLimitReached;
  // ADC the motor potentiometer is connected to (for the prototype)
  int adcPin;
  // enhanced quadrature encoder pulse module for the radial actuator
  eQEP radialMotoreQEP;
  long rm_eqep_period;
  //# End User Private Variables Marker
};

#endif
