#ifndef RECEIVER_HPP
#define RECEIVER_HPP
#include "ros/ros.h"
#include "node/Component.hpp"
#include "pub_sub_tg/message.h"

//# Start User Includes Marker
#include "network/receiver.hpp"
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class receiver : public Component
{
public:
  // Constructor
  receiver(ComponentConfig& _config, int argc, char **argv) : Component(_config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Subscriber Callback - message_sub
  void message_sub_OnOneData(const pub_sub_tg::message::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~receiver();

private:

  // Subscriber
  ros::Subscriber message_sub; 

  //# Start User Private Variables Marker
  uint64_t id;
  Network::receiver ddos;
  //# End User Private Variables Marker
};

#endif
