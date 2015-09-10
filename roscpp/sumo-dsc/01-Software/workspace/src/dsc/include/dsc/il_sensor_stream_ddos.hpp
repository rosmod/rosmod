#ifndef IL_SENSOR_STREAM_DDOS_HPP
#define IL_SENSOR_STREAM_DDOS_HPP
#include "ros/ros.h"
#include "node/Component.hpp"
#include "dsc/sensor_state.h"



//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class il_sensor_stream_ddos : public Component
{
public:
  // Constructor
  il_sensor_stream_ddos(ComponentConfig& _config, int argc, char **argv) : Component(_config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~il_sensor_stream_ddos();

private:

  // Publisher 
  ros::Publisher il_sensor_stream_ddos_pub;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
