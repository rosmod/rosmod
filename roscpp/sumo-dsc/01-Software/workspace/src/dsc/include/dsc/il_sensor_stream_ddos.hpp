#ifndef IL_SENSOR_STREAM_DDOS_HPP
#define IL_SENSOR_STREAM_DDOS_HPP
#include "ros/ros.h"
#include "node/Component.hpp"
#include "dsc/sensor_state.h"


#include "network/sender.hpp"

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

  // do we abide by the profiles?
  bool tg_misbehave;
  // size of messages generated
  uint64_t max_data_length;
  // Publisher 
  ros::Publisher il_sensor_stream_ddos_pub;
  // Timer for generating traffic
  ros::Timer il_sensor_stream_ddos_pub_timer;
  // Timer callback for traffic generation
  void il_sensor_stream_ddos_pub_timerCallback(const ros::TimerEvent& event);
  // publisher sender middleware
  Network::sender il_sensor_stream_ddos_pub_send_mw;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
