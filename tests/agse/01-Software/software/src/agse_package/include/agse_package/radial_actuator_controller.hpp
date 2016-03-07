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
  //# End User Private Variables Marker
};

#endif
