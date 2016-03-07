#ifndef USER_INPUT_CONTROLLER_HPP
#define USER_INPUT_CONTROLLER_HPP
#include "node/Component.hpp"
#include "agse_package/controlInputs.h"
#include "agse_package/armState.h"

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

class user_input_controller : public Component
{
public:
  // Constructor
  user_input_controller(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - armState_sub
  void armState_sub_operation(const agse_package::armState::ConstPtr& received_data); 
 
  // Timer Operation - userInputTimer
  void userInputTimer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~user_input_controller();

private:

  // Timer
  NAMESPACE::Timer userInputTimer;

  // Subscriber
  NAMESPACE::Subscriber armState_sub;

  // Publisher 
  NAMESPACE::Publisher controlInputs_pub;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
