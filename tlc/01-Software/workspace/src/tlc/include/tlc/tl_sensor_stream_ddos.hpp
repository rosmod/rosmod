#ifndef TL_SENSOR_STREAM_DDOS_HPP
#define TL_SENSOR_STREAM_DDOS_HPP
#include "node/Component.hpp"
#include "tlc/ryg_state.h"

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

class tl_sensor_stream_ddos : public Component
{
public:
  // Constructor
  tl_sensor_stream_ddos(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~tl_sensor_stream_ddos();

private:

  // Publisher 
  NAMESPACE::Publisher tl_sensor_stream_ddos_pub;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
