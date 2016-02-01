#ifndef RECEIVER_HPP
#define RECEIVER_HPP
#include "ros/ros.h"
#include "node/Component.hpp"
#include "pkg/routed_msg.h"


#include "network/receiver.hpp"

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class receiver : public Component
{
public:
  // Constructor
  receiver(ComponentConfig& _config, int argc, char **argv) : Component(_config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Subscriber Callback - routed_msg_sub
  void routed_msg_sub_OnOneData(const pkg::routed_msg::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~receiver();

private:

  // callback func for when servers are done receiving data
  void mw_recv_done_callback(Network::receiver* receiver_mw);
  // Subscriber
  ros::Subscriber routed_msg_sub;
  // message id for this data stream
  uint64_t routed_msg_sub_id;
  // subscriber receiver middleware
  Network::receiver routed_msg_sub_recv_mw;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
