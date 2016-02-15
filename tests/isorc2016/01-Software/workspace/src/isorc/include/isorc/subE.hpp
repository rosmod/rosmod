#ifndef SUBE_HPP
#define SUBE_HPP
#include "node/Component.hpp"
#include "isorc/messageE.h"

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

class subE : public Component
{
public:
  // Constructor
  subE(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - e_sub
  void e_sub_operation(const isorc::messageE::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~subE();

private:

  // function for when servers are done receiving data
  void mw_recv_done_operation(Network::receiver* receiver_mw);
  // Subscriber
  NAMESPACE::Subscriber e_sub;
  // message id for this data stream
  uint64_t e_sub_id;
  // subscriber receiver middleware
  Network::receiver e_sub_recv_mw;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
