#ifndef SUBA_HPP
#define SUBA_HPP
#include "node/Component.hpp"
#include "isorc/messageA.h"

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

class subA : public Component
{
public:
  // Constructor
  subA(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - a_sub
  void a_sub_operation(const isorc::messageA::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~subA();

private:

  // function for when servers are done receiving data
  void mw_recv_done_operation(Network::receiver* receiver_mw);
  // Subscriber
  NAMESPACE::Subscriber a_sub;
  // message id for this data stream
  uint64_t a_sub_id;
  // subscriber receiver middleware
  Network::receiver a_sub_recv_mw;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
