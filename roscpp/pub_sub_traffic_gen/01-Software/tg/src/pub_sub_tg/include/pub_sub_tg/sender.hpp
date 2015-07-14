#ifndef SENDER_HPP
#define SENDER_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "pub_sub_tg/message.h"
#include "pub_sub_tg/oob_comm.h"


//# Start User Includes Marker
#include "pub_sub_tg/NetworkProfile.hpp"
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class sender : public Component
{
public:
  // Constructor
  sender(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

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
  uint64_t uuid;
  std::string profileName;
  NetworkProfile profile;
  bool deactivated;
  bool metered;
  void message_pub_wrapper(const pub_sub_tg::message& msg);
  
  //# End User Private Variables Marker
};

#endif
