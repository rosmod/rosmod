#ifndef TL_ACTUATOR_HPP
#define TL_ACTUATOR_HPP
#include "node/Component.hpp"
#include "tlc/ryg_control.h"
#include "tlc/tlc_set_ryg_state.h"

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

class tl_actuator : public Component
{
public:
  // Constructor
  tl_actuator(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - ryg_control_sub
  void ryg_control_sub_operation(const tlc::ryg_control::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~tl_actuator();

private:

  // Subscriber
  NAMESPACE::Subscriber ryg_control_sub;

  // Client 
  NAMESPACE::ServiceClient tlc_set_ryg_state_client;

  //# Start User Private Variables Marker
  std::string _id;
  //# End User Private Variables Marker
};

#endif
