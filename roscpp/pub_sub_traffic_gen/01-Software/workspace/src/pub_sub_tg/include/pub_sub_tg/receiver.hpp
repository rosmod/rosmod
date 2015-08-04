#ifndef RECEIVER_HPP
#define RECEIVER_HPP
#include "ros/ros.h"
#include "Component.hpp"
#include "pub_sub_tg/message.h"
#include "pub_sub_tg/oob_comm.h"


//# Start User Includes Marker
#include "network/NetworkProfile.hpp"
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

  // Client 
  ros::ServiceClient oob_client;

  //# Start User Private Variables Marker
  ros::ServiceClient oob_client_pub1;
  ros::ServiceClient oob_client_pub2;
  ros::ServiceClient oob_client_pub3;

  Network::NetworkProfile profile;
  ros::Timer tgTimer;
  ros::Time endTime;
  std::vector<Network::Message> messages;
  uint64_t id;

  std::map<uint64_t,ros::ServiceClient*> oob_map;
  std::map<uint64_t,Network::NetworkProfile> profile_map;

  void TrafficGeneratorTimer(const ros::TimerEvent& event);
  //# End User Private Variables Marker
};

#endif
