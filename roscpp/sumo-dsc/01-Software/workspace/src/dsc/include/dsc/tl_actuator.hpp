#ifndef TL_ACTUATOR_HPP
#define TL_ACTUATOR_HPP
#include "ros/ros.h"
#include "node/Component.hpp"
#include "dsc/ryg_control.h"
#include "dsc/sumo_tlc_set_ryg_state.h"


#include "network/receiver.hpp"

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class tl_actuator : public Component
{
public:
  // Constructor
  tl_actuator(ComponentConfig& _config, int argc, char **argv) : Component(_config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Subscriber Callback - ryg_control_sub
  void ryg_control_sub_OnOneData(const dsc::ryg_control::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~tl_actuator();

private:

  // callback func for when servers are done receiving data
  void mw_recv_done_callback(Network::receiver* receiver_mw);
  // Subscriber
  ros::Subscriber ryg_control_sub;
  // message id for this data stream
  uint64_t ryg_control_sub_id;
  // subscriber receiver middleware
  Network::receiver ryg_control_sub_recv_mw;

  // Client 
  ros::ServiceClient tlc_set_ryg_state_client;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
