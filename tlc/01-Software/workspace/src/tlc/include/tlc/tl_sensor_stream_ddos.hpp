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


#include "network/sender.hpp"

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

  // do we abide by the profiles?
  bool tg_misbehave;
  // size of messages generated
  uint64_t max_data_length;
  // Publisher 
  NAMESPACE::Publisher tl_sensor_stream_ddos_pub;
  // Timer for generating traffic
  NAMESPACE::Timer tl_sensor_stream_ddos_pub_timer;
  // Timer callback for traffic generation
  void tl_sensor_stream_ddos_pub_timerCallback(const NAMESPACE::TimerEvent& event);
  // publisher sender middleware
  Network::sender tl_sensor_stream_ddos_pub_send_mw;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
