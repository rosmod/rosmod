#ifndef NETWORK_SENDER_HPP
#define NETWORK_SENDER_HPP

#include "ros/ros.h"

#include "network/NetworkProfile.hpp"
#include <std_msgs/UInt64MultiArray.h>

namespace Network
{
  class Exceeded_Production_profile {}; // sender-side exception

  template <typename T>
  class sender
  {
  public:
    sender();
    void init(std::string profileName);
    void set_duration(double dur);
    void set_output_filename(std::string filename);
    void send(ros::Publisher pub, const T& msg);
    void oob_recv();

  public:
    std::vector<Network::Message> messages;
    Network::NetworkProfile profile;
    ros::Duration duration;
    uint64_t uuid;
    ros::Time endTime;

  private:
    ros::Time nextSendTime;

    ros::Subscriber oob_sub;
    bool deactivated;

    std::string output_filename;
  };
};

#endif
