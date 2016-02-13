#ifndef RECEIVERC_HPP
#define RECEIVERC_HPP
#include "node/Component.hpp"
#include "multi_node_sample/messageC.h"

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

class receiverC : public Component
{
public:
  // Constructor
  receiverC(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - subscriberC
  void subscriberC_operation(const multi_node_sample::messageC::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~receiverC();

private:

  // function for when servers are done receiving data
  void mw_recv_done_operation(Network::receiver* receiver_mw);
  // Subscriber
  NAMESPACE::Subscriber subscriberC;
  // message id for this data stream
  uint64_t subscriberC_id;
  // subscriber receiver middleware
  Network::receiver subscriberC_recv_mw;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
