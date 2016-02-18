#ifndef COMM_HPP
#define COMM_HPP
#include "node/Component.hpp"
#include "pubsub/pubsubTopic.h"
#include "pubsub/pubsubTopic.h"

#ifdef USE_ROSMOD
  #include "rosmod/rosmod_ros.h"
#else
  #ifdef USE_ROSCPP
    #include "ros/ros.h"
  #endif
#endif


#include "network/sender.hpp"
#include "network/receiver.hpp"

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class comm : public Component
{
public:
  // Constructor
  comm(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - sub
  void sub_operation(const pubsub::pubsubTopic::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~comm();

private:

  // function for when servers are done receiving data
  void mw_recv_done_operation(Network::receiver* receiver_mw);
  // Subscriber
  NAMESPACE::Subscriber sub;
  // message id for this data stream
  uint64_t sub_id;
  // subscriber receiver middleware
  Network::receiver sub_recv_mw;

  // do we abide by the profiles?
  bool tg_misbehave;
  // size of messages generated
  uint64_t max_data_length;
  // Publisher 
  NAMESPACE::Publisher pub;
  // Timer for generating traffic
  NAMESPACE::Timer pub_timer;
  // Timer operation for traffic generation
  void pub_timer_operation(const NAMESPACE::TimerEvent& event);
  // publisher sender middleware
  Network::sender pub_send_mw;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
