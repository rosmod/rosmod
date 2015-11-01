#ifndef CONTROL_STREAM_DDOS_HPP
#define CONTROL_STREAM_DDOS_HPP
#include "node/Component.hpp"
#include "tlc/ryg_control.h"

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

class control_stream_ddos : public Component
{
public:
  // Constructor
  control_stream_ddos(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~control_stream_ddos();

private:

  // Publisher 
  NAMESPACE::Publisher control_stream_ddos_pub;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
