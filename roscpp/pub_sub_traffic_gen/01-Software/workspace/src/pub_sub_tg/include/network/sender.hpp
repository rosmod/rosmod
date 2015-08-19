#ifndef NETWORK_SENDER_HPP
#define NETWORK_SENDER_HPP

#include "ros/ros.h"

#include "network/NetworkProfile.hpp"
#include <std_msgs/UInt64MultiArray.h>

namespace Network
{
  static const std::string oob_multicast_group = "224.0.0.251";
  static const int oob_multicast_port = 12345;
  
  class Exceeded_Production_profile {}; // sender-side exception

  class sender
  {
  public:
    sender();
    void init(std::string profileName);

    void set_duration(double dur) { endTime = ros::Time::now() + ros::Duration(dur); }
    void set_output_filename(std::string filename) { output_filename = filename; }

    uint64_t get_uuid() const { return uuid; }
    ros::Time get_end_time() const { return ros::Time(endTime); }

    void record();

    template <typename T>
    double send(ros::Publisher pub, const T& msg);
    void oob_recv();

  public:
    std::vector<Network::Message> messages;
    Network::NetworkProfile profile;

  private:
    bool deactivated;
    ros::Time nextSendTime;
    uint64_t uuid;
    ros::Time endTime;
    uint64_t id;

    int oob_recv_sockfd;

    std::string output_filename;
  };
};

#endif
