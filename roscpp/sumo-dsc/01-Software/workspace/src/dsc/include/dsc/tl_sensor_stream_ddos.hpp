#ifndef TL_SENSOR_STREAM_DDOS_HPP
#define TL_SENSOR_STREAM_DDOS_HPP
#include "ros/ros.h"
#include "node/Component.hpp"
#include "dsc/ryg_state.h"



//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class tl_sensor_stream_ddos : public Component
{
public:
  // Constructor
  tl_sensor_stream_ddos(ComponentConfig& _config, int argc, char **argv) : Component(_config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~tl_sensor_stream_ddos();

private:

  // Publisher 
  ros::Publisher tl_sensor_stream_ddos_pub;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
