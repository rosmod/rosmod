#ifndef SERVO_CONTROLLER_HPP
#define SERVO_CONTROLLER_HPP
#include "node/Component.hpp"
#include "agse_package/controlInputs.h"
#include "agse_package/armRotation.h"
#include "agse_package/gripperPos.h"
#include "agse_package/gripperRotation.h"

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

class servo_controller : public Component
{
public:
  // Constructor
  servo_controller(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - controlInputs_sub
  void controlInputs_sub_operation(const agse_package::controlInputs::ConstPtr& received_data); 
 
  // Server Operation - armRotation_server
  bool armRotation_operation(agse_package::armRotation::Request &req, 
    agse_package::armRotation::Response &res);

  // Server Operation - gripperPos_server
  bool gripperPos_operation(agse_package::gripperPos::Request &req, 
    agse_package::gripperPos::Response &res);

  // Server Operation - gripperRotation_server
  bool gripperRotation_operation(agse_package::gripperRotation::Request &req, 
    agse_package::gripperRotation::Response &res);

  // Timer Operation - servoTimer
  void servoTimer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~servo_controller();

private:

  // Timer
  NAMESPACE::Timer servoTimer;

  // Subscriber
  NAMESPACE::Subscriber controlInputs_sub;

  // Server 
  NAMESPACE::ServiceServer armRotation_server;

  // Server 
  NAMESPACE::ServiceServer gripperPos_server;

  // Server 
  NAMESPACE::ServiceServer gripperRotation_server;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
