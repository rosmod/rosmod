#ifndef RECEIVER_HPP
#define RECEIVER_HPP
#include "node/Component.hpp"
#include "simple/pubsubTopic.h"

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

class receiver : public Component
{
public:
  // Constructor
  receiver(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - sub
  void sub_operation(const simple::pubsubTopic::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~receiver();

private:

  // function for when servers are done receiving data
  void mw_recv_done_operation(Network::receiver* receiver_mw);
  // Subscriber
  NAMESPACE::Subscriber sub;
  // message id for this data stream
  uint64_t sub_id;
  // subscriber receiver middleware
  Network::receiver sub_recv_mw;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
