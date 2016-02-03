#ifndef PUBLISHER_HPP
#define PUBLISHER_HPP
#include "node/Component.hpp"
#include "lro_package/Message.h"

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

class Publisher : public Component
{
public:
  // Constructor
  Publisher(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Timer Operation - publish_timer
  void publish_timer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Publisher();

private:

  // Timer
  NAMESPACE::Timer publish_timer;

  // Publisher 
  NAMESPACE::Publisher publisher_port;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
