#ifndef CONTROL_STREAM_DDOS_HPP
#define CONTROL_STREAM_DDOS_HPP
#include "ros/ros.h"
#include "node/Component.hpp"
#include "dsc/ryg_control.h"


#include "network/sender.hpp"

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class control_stream_ddos : public Component
{
public:
  // Constructor
  control_stream_ddos(ComponentConfig& _config, int argc, char **argv) : Component(_config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~control_stream_ddos();

private:

  // do we abide by the profiles?
  bool tg_misbehave;
  // size of messages generated
  uint64_t max_data_length;
  // Publisher 
  ros::Publisher control_stream_ddos_pub;
  // Timer for generating traffic
  ros::Timer control_stream_ddos_pub_timer;
  // Timer callback for traffic generation
  void control_stream_ddos_pub_timerCallback(const ros::TimerEvent& event);
  // publisher sender middleware
  Network::sender control_stream_ddos_pub_send_mw;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
