#ifndef PUBB_HPP
#define PUBB_HPP
#include "node/Component.hpp"
#include "isorc/messageB.h"

#ifdef USE_ROSMOD
  #include "rosmod/rosmod_ros.h"
#else
  #ifdef USE_ROSCPP
    #include "ros/ros.h"
  #endif
#endif


#include "network/sender.hpp"

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class pubB : public Component
{
public:
  // Constructor
  pubB(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Timer Operation - pub_timer
  void pub_timer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~pubB();

private:

  // Timer
  NAMESPACE::Timer pub_timer;

  // do we abide by the profiles?
  bool tg_misbehave;
  // size of messages generated
  uint64_t max_data_length;
  // Publisher 
  NAMESPACE::Publisher b_pub;
  // Timer for generating traffic
  NAMESPACE::Timer b_pub_timer;
  // Timer operation for traffic generation
  void b_pub_timer_operation(const NAMESPACE::TimerEvent& event);
  // publisher sender middleware
  Network::sender b_pub_send_mw;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
