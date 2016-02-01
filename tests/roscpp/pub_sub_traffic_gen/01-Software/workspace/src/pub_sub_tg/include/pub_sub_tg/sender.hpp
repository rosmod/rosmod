#ifndef SENDER_HPP
#define SENDER_HPP
#include "ros/ros.h"
#include "node/Component.hpp"
#include "pub_sub_tg/message.h"
#include "pub_sub_tg/oob_comm.h"


//# Start User Includes Marker
#include "network/sender.hpp"
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

  // Publisher 
  ros::Publisher message_pub;

  //# Start User Private Variables Marker
  // NETWORK MIDDLEWARE INFO
  ros::Timer tgTimer;

  Network::sender sender_middleware;
  uint64_t max_data_length;

  void TrafficGeneratorTimer(const ros::TimerEvent& event);
  // END NETWORK MIDDLEWARE INFO
  //# End User Private Variables Marker
};

#endif
