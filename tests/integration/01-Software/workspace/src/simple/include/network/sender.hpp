#ifndef NETWORK_SENDER_HPP
#define NETWORK_SENDER_HPP

#include "ros/ros.h"

#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "network/NetworkProfile.hpp"
#include "network/oob.hpp"

namespace Network
{
  class Exceeded_Production_profile {}; // sender-side exception

  class sender
  {
  public:
    sender()
    {
      deactivated = false;
      nextSendTime = ros::Time::now();
    }

    int create_oob_mc_socket()
    {
      // create the multicast receive socket
      sd = socket(AF_INET, SOCK_DGRAM, 0);
      if(sd < 0)
	{
	  perror("Opening datagram socket error");
	  exit(1);
	}
      else
	printf("Opening datagram socket....OK.\n");

      /* Enable SO_REUSEADDR to allow multiple instances of this */
      /* application to receive copies of the multicast datagrams. */
      {
	int reuse = 1;
	if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0)
	  {
	    perror("Setting SO_REUSEADDR error");
	    close(sd);
	    exit(1);
	  }
	else
	  printf("Setting SO_REUSEADDR...OK.\n");
      }

      /* Bind to the proper port number with the IP address */
      /* specified as INADDR_ANY. */
      struct sockaddr_in localSock;
      memset((char *) &localSock, 0, sizeof(localSock));
      localSock.sin_family = AF_INET;
      localSock.sin_port = htons(oob_mc_port);
      localSock.sin_addr.s_addr = INADDR_ANY;
      if(bind(sd, (struct sockaddr*)&localSock, sizeof(localSock)))
	{
	  perror("Binding datagram socket error");
	  close(sd);
	  exit(1);
	}
      else
	printf("Binding datagram socket...OK.\n");

      /* Join the multicast group 226.1.1.1 on the local 203.106.93.94 */
      /* interface. Note that this IP_ADD_MEMBERSHIP option must be */
      /* called for each local interface over which the multicast */
      /* datagrams are to be received. */
      struct ip_mreq group;
      group.imr_multiaddr.s_addr = inet_addr(oob_mc_group.c_str());
      group.imr_interface.s_addr = inet_addr("0.0.0.0");
      if(setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0)
	{
	  perror("Adding multicast group error");
	  close(sd);
	  exit(1);
	}
      else
	printf("Adding multicast group...OK.\n");

      boost::thread *io_thread =
	new boost::thread( boost::bind(&sender::oob_recv_threadfunc, this) );
      return 0;
    }

    int init(int argc, char **argv, std::string prof_str, uint64_t u = -1, bool enable_oob = false)
    {
      profile.initializeFromString((char *)prof_str.c_str());
      if (u > 0)
	uuid = u;
      else
	uuid = profile.uuid;

      if (enable_oob)
	create_oob_mc_socket();

      return 0;
    }

    void set_duration(double dur) { endTime = ros::Time::now() + ros::Duration(dur); }
    void set_output_filename(std::string filename) { output_filename = filename; }
    void set_uuid( uint64_t u ) { uuid = u; }

    uint64_t get_uuid() const { return uuid; }
    ros::Time get_end_time() const { return ros::Time(endTime); }

    void record() { Network::write_data(output_filename.c_str(), messages); }

    template <typename T>
    double send(NAMESPACE::Publisher pub, const T& msg)
    {
      static uint64_t id = 0;
      double timeDiff = 0;
      // CHECK AGAINST PRIVATE VARIABLE : DEACTIVATED
      if (deactivated)
	throw Network::Exceeded_Production_Profile();

      uint64_t msgSizeBytes =
	ros::serialization::Serializer<T>::serializedLength(msg);
      msgSizeBytes += Network::ipv4_header_bytes;
      msgSizeBytes += Network::udp_header_bytes;
      msgSizeBytes += Network::ros_udp_header_bytes;
      // take a measurement
      Network::Message new_msg;
      new_msg.Bytes(msgSizeBytes);
      new_msg.Id(id);
      new_msg.TimeStamp();
      // record the measurement
      messages.push_back(new_msg);
      id++;

      // CHECK AGAINST PROFILE (INCLUDING METERING FROM RECEIVER)
      if (profile.Initialized() and profile.HasEntries()) // profile is not in error state
	{
	  ros::Time now = ros::Time::now();
	  timeDiff = (now - nextSendTime).toSec();
	  if (timeDiff < 0)
	    {
	      throw Network::Exceeded_Production_Profile();
	    }
      
	  timespec current_time;
	  current_time.tv_sec = now.sec;
	  current_time.tv_nsec = now.nsec;
	  timeDiff = profile.Delay(msgSizeBytes * 8, current_time);
	  nextSendTime = now + ros::Duration(timeDiff);
	}
      // IF EVERYTHING IS ALRIGHT, PASS IT THROUGH
      pub.publish(msg);
      return timeDiff;
    }

    void oob_recv_threadfunc()
    {
      while (1)
	{
	  int length = read(sd, data_, sizeof(data_));
	  if (length > 0)
	    {
	      bool has_uuid = false;
	      bool my_val = false;
	      // parse if for a list of UUIDs and their deactivated setting
	      std::string s = data_;
	      std::stringstream iss(s);
	      std::string substr;
	      while ( std::getline(iss, substr, ';') )
		{
		  std::stringstream t_iss(substr);
		  std::string u;
		  std::string val;
		  std::getline(t_iss, u, ',');
		  std::getline(t_iss, val);
		  uint64_t tmp_uuid = (uint64_t) atoi(u.c_str());
		  bool tmp_val = (bool) atoi(val.c_str());
		  if (tmp_uuid == uuid)
		    {
		      has_uuid = true;
		      my_val = tmp_val;
		      break;
		    }
		}
	      // if our uuid is in the list, set our deactivated variable accordingly
	      if ( has_uuid )
		{
		  deactivated = my_val;
		}
	    }
	  else
	    {
	      boost::this_thread::sleep(
					boost::posix_time::milliseconds(1));
	    }
	}
    }

  public:
    std::vector<Network::Message> messages;
    Network::NetworkProfile profile;

  private:
    bool deactivated;
    ros::Time nextSendTime;
    uint64_t uuid;
    ros::Time endTime;

    static const int max_recv_buffer_size = 2000;
    int sd;
    char data_[max_recv_buffer_size];

    std::string output_filename;
  };
};

#endif
