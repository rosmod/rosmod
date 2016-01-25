#ifndef PERIODIC_PUBLISHER_HPP
#define PERIODIC_PUBLISHER_HPP
#include "node/Component.hpp"
#include "ten_component/secret_message.h"

#ifdef USE_ROSMOD
  #include "rosmod/rosmod_ros.h"
#else
  #ifdef USE_ROSCPP
    #include "ros/ros.h"
  #endif
#endif



//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Periodic_Publisher : public Component
{
public:
  // Constructor
  Periodic_Publisher(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Timer Operation - half_sec_timer
  void half_sec_timer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Periodic_Publisher();

private:

  // Timer
  NAMESPACE::Timer half_sec_timer;

  // Publisher 
  NAMESPACE::Publisher secret_message_publisher;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
