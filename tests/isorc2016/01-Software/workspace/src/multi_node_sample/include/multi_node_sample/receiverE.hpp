#ifndef RECEIVERE_HPP
#define RECEIVERE_HPP
#include "node/Component.hpp"
#include "multi_node_sample/messageE.h"

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

class receiverE : public Component
{
public:
  // Constructor
  receiverE(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - subscriberE
  void subscriberE_operation(const multi_node_sample::messageE::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~receiverE();

private:

  // function for when servers are done receiving data
  void mw_recv_done_operation(Network::receiver* receiver_mw);
  // Subscriber
  NAMESPACE::Subscriber subscriberE;
  // message id for this data stream
  uint64_t subscriberE_id;
  // subscriber receiver middleware
  Network::receiver subscriberE_recv_mw;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
