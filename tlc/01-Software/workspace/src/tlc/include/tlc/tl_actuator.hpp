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


#include "network/receiver.hpp"

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

  // callback func for when servers are done receiving data
  void mw_recv_done_operation(Network::receiver* receiver_mw);
  // Subscriber
  NAMESPACE::Subscriber ryg_control_sub;
  // message id for this data stream
  uint64_t ryg_control_sub_id;
  // subscriber receiver middleware
  Network::receiver ryg_control_sub_recv_mw;

  // Client 
  NAMESPACE::ServiceClient tlc_set_ryg_state_client;

  //# Start User Private Variables Marker
  std::string _id;
  //# End User Private Variables Marker
};

#endif
