#ifndef NETWORK_RECEIVER_HPP
#define NETWORK_RECEIVER_HPP

#include <arpa/inet.h>
#include "ros/ros.h"

#include "network/NetworkProfile.hpp"
#include <boost/thread/thread.hpp>

#include "network/buffer.hpp"

namespace Network
{
  static const std::string oob_mc_group = "224.0.0.251";
  static const int oob_mc_port = 12345;
  
  class receiver
  {
  public:
    receiver()
    {
      received_data = false;

      // CREATE THREAD HERE FOR RECEIVING DATA
      boost::thread *tmr_thread =
	new boost::thread( boost::bind(&receiver::buffer_recv_threadfunc, this) );
    }

    int init(std::string profileName, uint64_t buffer_capacity_bits)
    {
      profile.initializeFromFile(profileName.c_str());
      buffer.set_capacityBits(buffer_capacity_bits);

      // NEED TO CREATE MC SOCKET FOR SENDING OOB DATA
      if ( (oob_mc_send_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) <0)
	{
	  perror("socket");
	  return -1;
	}
      memset(&oob_mc_send_sockaddr, 0, sizeof(oob_mc_send_sockaddr));
      oob_mc_send_sockaddr.sin_family = AF_INET;
      oob_mc_send_sockaddr.sin_addr.s_addr = inet_addr(oob_mc_group.c_str());
      oob_mc_send_sockaddr.sin_port = htons(oob_mc_port);

      return 0;
    }

    void add_sender(std::string profileName)
    {
      Network::NetworkProfile p;
      p.initializeFromFile(profileName.c_str());

      uuids.push_back(p.uuid);
      profile_map[p.uuid] = p;
      receive_map[p.uuid] = std::map<ros::Time, uint64_t>();
    }

    void set_duration(double dur) { duration = ros::Duration(dur); }
    void set_output_filename(std::string filename) { output_filename = filename; }

    void update_sender_stream(uint64_t uuid, ros::Time t, uint64_t new_size)
    {
      uint64_t prevData = 0;
      if ( receive_map[uuid].size() )
	prevData = receive_map[uuid].end()->second;
      receive_map[uuid][t] = new_size + prevData;
    }

    ros::Time get_end_time() const { return ros::Time(endTime); }

    int oob_send(std::vector<uint64_t>& send_uuids, bool val)
    {
      int num_disabled = send_uuids.size();

      // FORMAT DATA
      char msg[(num_disabled+2) * 16];
      memset(msg,0,(num_disabled+2)*16);
      for (auto it = send_uuids.begin(); it != send_uuids.end(); ++it)
	{
	  sprintf(msg,"%s%lu,%d;", msg, *it, (int)val);
	}

      // SEND FORMATTED DATA
      if ( (sendto( oob_mc_send_sockfd,
		    msg,
		    strlen(msg),
		    0,
		    (struct sockaddr*)&oob_mc_send_sockaddr,
		    sizeof(oob_mc_send_sockaddr) )
	    ) < 0 )
	{
	  perror("sendto");
	  return -1;
	}
      printf("send data to %lu senders!\n",send_uuids.size());
      return 0;
    }

    void buffer_recv_threadfunc(void)
    {
      while ( true )
	{
	  // UPDATE SENDERS IF BUFFER IS CLEARING:
	  uint64_t currentSize = buffer.bits();
	  uint64_t currentCapacity = buffer.capacityBits();
	  if ( currentCapacity > 0 )
	    {
	      double utilization = (double)currentSize/(double)currentCapacity;
	      if ( utilization < 0.75 )
		unlimit_ddos();
	    }
	  // READ FROM THE BUFFER
	  double timerDelay = -1;
	  try
	    {
	      Network::Message msg = buffer.receive(1000);
	      if (!received_data)
		{
		  ros::Time now = ros::Time::now();
		  endTime = now + duration;
		  received_data = true;
		}
	      msg.TimeStamp();
	      messages.push_back(msg);
	      // CHECK AGAINST RECEIVER PROFILE: LOOK UP WHEN I CAN READ NEXT
	      timerDelay = profile.Delay(msg.Bits(), msg.LastEpochTime());
	    }
	  catch ( Network::Buffer_Empty& ex )
	    {
	    }
	  if ( ros::Time::now() >= endTime )
	    {
	      Network::write_data(output_filename.c_str(),messages);
	      break;
	    }
	  else
	    {
	      // SLEEP UNTIL NEXT TIME BUFFER CAN BE SERVICED
	      if (timerDelay > 0)
		ros::Duration(timerDelay).sleep();
	    }
	}
    }

    int unlimit_ddos(void)
    {
      if ( oob_send(disabled_uuids, false) < 0 )
	return -1;
      disabled_uuids.clear();
      return 0;
    }
    int limit_ddos(ros::Time now, double timeWindow)
    {
      std::vector<uint64_t> bad_uuids;
      ros::Time prevTime = now - ros::Duration(timeWindow);
      for (auto uuid_it = uuids.begin();
	   uuid_it != uuids.end(); ++uuid_it)
	{
	  uint64_t d1 = receive_map[*uuid_it].lower_bound(prevTime)->second;
	  ros::Time t1 = receive_map[*uuid_it].lower_bound(prevTime)->first;
	  uint64_t d2 = receive_map[*uuid_it].end()->second;
	  ros::Time t2 = receive_map[*uuid_it].end()->first;
	  // get data @ t1 from profile, get data @ t2 from profile
	  uint64_t pd1 = profile_map[*uuid_it].getDataAtTime({t1.sec, t1.nsec});
	  uint64_t pd2 = profile_map[*uuid_it].getDataAtTime({t2.sec, t2.nsec});
	  // if profile difference < (d2-d1) : they are sending too much
	  uint64_t pDiff = pd2 - pd1;
	  uint64_t diff = d2 - d1;
	  if (diff > pDiff) {
	    bad_uuids.push_back(*uuid_it);
	    if ( std::find(disabled_uuids.begin(),
			   disabled_uuids.end(),
			   *uuid_it) == disabled_uuids.end() )
	      disabled_uuids.push_back(*uuid_it);
	  }
	}
      return oob_send(bad_uuids, true);
    }

  public:
    message_buffer<Network::Message> buffer;
    Network::NetworkProfile profile;
    
  private:
    std::vector<Network::Message> messages;
    ros::Duration duration;
    ros::Time endTime;
    bool received_data;

    struct sockaddr_in oob_mc_send_sockaddr;
    int oob_mc_send_sockfd;

    std::string output_filename;

    std::vector<uint64_t> uuids;
    std::vector<uint64_t> disabled_uuids;
    std::map<uint64_t, Network::NetworkProfile> profile_map;
    std::map<uint64_t, std::map<ros::Time, uint64_t>> receive_map; // uuid - > <time, data>
  };
};

#endif
