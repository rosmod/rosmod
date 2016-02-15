#ifndef SUBD_HPP
#define SUBD_HPP
#include "node/Component.hpp"
#include "isorc/messageD.h"

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

class subD : public Component
{
public:
  // Constructor
  subD(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - d_sub
  void d_sub_operation(const isorc::messageD::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~subD();

private:

  // function for when servers are done receiving data
  void mw_recv_done_operation(Network::receiver* receiver_mw);
  // Subscriber
  NAMESPACE::Subscriber d_sub;
  // message id for this data stream
  uint64_t d_sub_id;
  // subscriber receiver middleware
  Network::receiver d_sub_recv_mw;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
