#ifndef SENDER_HPP
#define SENDER_HPP
#include "ros/ros.h"
#include "node/Component.hpp"
#include "pkg/routed_msg.h"


#include "network/sender.hpp"

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class sender : public Component
{
public:
  // Constructor
  sender(ComponentConfig& _config, int argc, char **argv) : Component(_config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~sender();

private:

  // do we abide by the profiles?
  bool tg_misbehave;
  // size of messages generated
  uint64_t max_data_length;
  // Publisher 
  ros::Publisher routed_msg_pub;
  // Timer for generating traffic
  ros::Timer routed_msg_pub_timer;
  // Timer callback for traffic generation
  void routed_msg_pub_timerCallback(const ros::TimerEvent& event);
  // publisher sender middleware
  Network::sender routed_msg_pub_send_mw;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
