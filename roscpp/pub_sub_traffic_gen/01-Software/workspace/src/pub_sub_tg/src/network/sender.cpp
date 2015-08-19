#include "network/sender.hpp"

using namespace Network;

sender::sender()
{
  deactivated = false;
  id = 0;
}

int sender::init(std::string profileName)
{
  profile.initializeFromFile(profileName.c_str());
  uuid = profile.uuid;

  // BIND TO MULTICAST SOCKET FOR RECEIVING OOB
  if ( (oob_mc_recv_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
      perror("socket");
      return -1;
    }
  memset(&oob_mc_recv_sockaddr, 0, sizeof(oob_mc_recv_sockaddr));
  oob_mc_recv_sockaddr.sin_family = AF_INET;
  oob_mc_recv_sockaddr.sin_port = htons(oob_mc_port);
  oob_mc_recv_sockaddr.sin_addr.s_addr = INADDR_ANY;
  if ( bind( oob_mc_recv_sockfd,
	     (struct sockaddr *)&oob_mc_recv_sockaddr,
	     sizeof(oob_mc_recv_sockaddr)) )
    {
      perror("binding");
      return -1;
    }
  struct ip_mreq group;
  group.imr_multiaddr.s_addr = inet_addr(oob_mc_group.c_str());
  group.imr_interface.s_addr = inet_addr("localhost");
  if ( setsockopt( oob_mc_recv_sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0 )
    {
      perror("multicast group");
      return -1;
    }
  // CREATE THREAD FOR RECEIVING OOB COMMUNICATIONS
  return 0;
}

template<typename T>
double sender::send(ros::Publisher pub, const T& msg)
{
  double timeDiff = 0;
  // CHECK AGAINST PRIVATE VARIABLE : DEACTIVATED
  if (deactivated)
    throw Network::Exceeded_Production_Profile();
  // CHECK AGAINST PROFILE (INCLUDING METERING FROM RECEIVER)
  if (profile.Initialized()) // profile is not in error state
    {
      uint64_t msgSizeBytes =
	ros::serialization::Serializer<T>::serializedLength(msg);
      // take a measurement
      Network::Message new_msg;
      new_msg.Bytes(msgSizeBytes);
      new_msg.Id(id);
      new_msg.TimeStamp();

      ros::Time now = ros::Time::now();
      timeDiff = (now - nextSendTime).toSec();
      if (timeDiff < 0)
	{
	  throw Network::Exceeded_Production_Profile();
	}
      
      // record the measurement
      messages.push_back(new_msg);
      id++;

      if (profile.resources.size() > 0) // has entries in profile
	{
	  timespec current_time;
	  current_time.tv_sec = now.sec;
	  current_time.tv_nsec = now.nsec;
	  timeDiff = profile.Delay(msgSizeBytes * 8, current_time);
	  nextSendTime = now + ros::Duration(timeDiff);
	}
    }
  // IF EVERYTHING IS ALRIGHT, PASS IT THROUGH
  pub.publish(msg);
  return timeDiff;
}

void sender::record(void)
{
  Network::write_data(output_filename.c_str(), messages);
}

void sender::oob_recv(void)
{
  // receive udp multicast message here
  // parse if for a list of UUIDs and their deactivated setting
  // if our uuid is in the list, set our deactivated variable accordingly
}
