#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "pub_sub_tg/message.h"
#include "pub_sub_tg/oob_comm.h"


//# Start User Includes Marker
#include "pub_sub_tg/NetworkProfile.hpp"
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class receiver : public Component
{
public:
  // Constructor
  receiver(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

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

  // Client 
  ros::ServiceClient oob_client;

  //# Start User Private Variables Marker
  std::vector<Network::Message> messages;
  uint64_t id;

  std::map<uint64_t,ros::ServiceClient*> oob_map;
  std::map<uint64_t,Network::NetworkProfile> profile_map;

  void message_sub_wrapper(const ros::MessageEvent<pub_sub_tg::message const>& event);
  //# End User Private Variables Marker
};

#endif
