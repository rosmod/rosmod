#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP
#include "node/Component.hpp"
#include "tlc/ryg_control.h"
#include "tlc/ryg_state.h"
#include "tlc/sensor_state.h"
#include "tlc/sensor_state.h"

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

class controller : public Component
{
public:
  // Constructor
  controller(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - ryg_state_sub
  void ryg_state_sub_operation(const tlc::ryg_state::ConstPtr& received_data); 
 
  // Subscriber Operation - e3_ingress
  void e3_ingress_operation(const tlc::sensor_state::ConstPtr& received_data); 
 
  // Subscriber Operation - e3_egress
  void e3_egress_operation(const tlc::sensor_state::ConstPtr& received_data); 
 
  // Timer Operation - controller_timer
  void controller_timer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~controller();

private:

  // Timer
  NAMESPACE::Timer controller_timer;

  // Subscriber
  NAMESPACE::Subscriber ryg_state_sub;

  // Subscriber
  NAMESPACE::Subscriber e3_ingress;

  // Subscriber
  NAMESPACE::Subscriber e3_egress;

  // Publisher 
  NAMESPACE::Publisher ryg_control_pub;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
