#ifndef NETWORK_SENDER_HPP
#define NETWORK_SENDER_HPP

#include <arpa/inet.h>
#include "ros/ros.h"

#include "network/NetworkProfile.hpp"
#include <std_msgs/UInt64MultiArray.h>

namespace Network
{
  static const std::string oob_mc_group = "224.0.0.251";
  static const int oob_mc_port = 12345;
  
  class Exceeded_Production_profile {}; // sender-side exception

  class sender
  {
  public:
    sender();
    int init(std::string profileName);

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

    int oob_mc_recv_sockfd;
    struct sockaddr_in oob_mc_recv_sockaddr;

    std::string output_filename;
  };
};

#endif
