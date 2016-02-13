#ifndef SENDERACDE_HPP
#define SENDERACDE_HPP
#include "node/Component.hpp"
#include "multi_node_sample/messageA.h"
#include "multi_node_sample/messageC.h"
#include "multi_node_sample/messageD.h"
#include "multi_node_sample/messageE.h"

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

class senderACDE : public Component
{
public:
  // Constructor
  senderACDE(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Timer Operation - timerA
  void timerA_operation(const NAMESPACE::TimerEvent& event);

  // Timer Operation - timerC
  void timerC_operation(const NAMESPACE::TimerEvent& event);

  // Timer Operation - timerD
  void timerD_operation(const NAMESPACE::TimerEvent& event);

  // Timer Operation - timerE
  void timerE_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~senderACDE();

private:

  // Timer
  NAMESPACE::Timer timerA;

  // Timer
  NAMESPACE::Timer timerC;

  // Timer
  NAMESPACE::Timer timerD;

  // Timer
  NAMESPACE::Timer timerE;

  // do we abide by the profiles?
  bool tg_misbehave;
  // size of messages generated
  uint64_t max_data_length;
  // Publisher 
  NAMESPACE::Publisher publisherA;
  // Timer for generating traffic
  NAMESPACE::Timer publisherA_timer;
  // Timer operation for traffic generation
  void publisherA_timer_operation(const NAMESPACE::TimerEvent& event);
  // publisher sender middleware
  Network::sender publisherA_send_mw;

  // Publisher 
  NAMESPACE::Publisher publisherC;
  // Timer for generating traffic
  NAMESPACE::Timer publisherC_timer;
  // Timer operation for traffic generation
  void publisherC_timer_operation(const NAMESPACE::TimerEvent& event);
  // publisher sender middleware
  Network::sender publisherC_send_mw;

  // Publisher 
  NAMESPACE::Publisher publisherD;
  // Timer for generating traffic
  NAMESPACE::Timer publisherD_timer;
  // Timer operation for traffic generation
  void publisherD_timer_operation(const NAMESPACE::TimerEvent& event);
  // publisher sender middleware
  Network::sender publisherD_send_mw;

  // Publisher 
  NAMESPACE::Publisher publisherE;
  // Timer for generating traffic
  NAMESPACE::Timer publisherE_timer;
  // Timer operation for traffic generation
  void publisherE_timer_operation(const NAMESPACE::TimerEvent& event);
  // publisher sender middleware
  Network::sender publisherE_send_mw;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
