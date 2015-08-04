#ifndef SENDER_HPP
#define SENDER_HPP
#include "ros/ros.h"
#include "Component.hpp"
#include "pub_sub_tg/message.h"
#include "pub_sub_tg/oob_comm.h"


//# Start User Includes Marker
#include "network/NetworkProfile.hpp"
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

  // Server Callback - oob_server
  bool oob_commCallback(pub_sub_tg::oob_comm::Request &req, 
    pub_sub_tg::oob_comm::Response &res);

  // Start up
  void startUp();

  // Destructor
  ~sender();

private:

  // Publisher 
  ros::Publisher message_pub;

  // Server 
  ros::ServiceServer oob_server;

  //# Start User Private Variables Marker

  // NETWORK MIDDLEWARE INFO
  ros::Timer tgTimer;
  ros::Time endTime;

  uint64_t uuid;

  std::string profileName;
  Network::NetworkProfile profile;
  ros::Time nextSendTime;
  bool deactivated;
  bool metered;

  std::vector<Network::Message> messages;
  uint64_t id;
  uint64_t max_data_length;

  void message_pub_wrapper(const pub_sub_tg::message& msg);
  void TrafficGeneratorTimer(const ros::TimerEvent& event);
  
  //# End User Private Variables Marker
};

#endif
