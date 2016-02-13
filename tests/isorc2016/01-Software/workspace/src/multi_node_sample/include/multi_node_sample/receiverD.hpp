#ifndef RECEIVERD_HPP
#define RECEIVERD_HPP
#include "node/Component.hpp"
#include "multi_node_sample/messageD.h"

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

class receiverD : public Component
{
public:
  // Constructor
  receiverD(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - subscriberD
  void subscriberD_operation(const multi_node_sample::messageD::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~receiverD();

private:

  // function for when servers are done receiving data
  void mw_recv_done_operation(Network::receiver* receiver_mw);
  // Subscriber
  NAMESPACE::Subscriber subscriberD;
  // message id for this data stream
  uint64_t subscriberD_id;
  // subscriber receiver middleware
  Network::receiver subscriberD_recv_mw;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
