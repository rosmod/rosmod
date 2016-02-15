#ifndef NETWORK_RECEIVER_HPP
#define NETWORK_RECEIVER_HPP

#include "ros/ros.h"

#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "network/NetworkProfile.hpp"
#include "network/buffer.hpp"
#include "network/oob.hpp"

namespace Network
{
  class receiver
  {
  public:
    receiver()
      : enable_sendback(false),
	received_data(false)
    {
    }

    int init(int argc, char ** argv, std::string prof_str, uint64_t buff_capacity_bits = 0, bool enable_oob = false)
    {
      this->buffer_capacity_bits = buff_capacity_bits;
      profile.initializeFromString((char *)prof_str.c_str());
      buffer.set_capacityBits(this->buffer_capacity_bits);

      set_enable_sendback(enable_oob);
      if (enable_sendback)
	init_oob();

      boost::thread *tmr_thread =
	new boost::thread( boost::bind(&receiver::buffer_recv_threadfunc, this) );

      return 0;
    }

    void add_sender(uint64_t id, std::string profile)
    {
      Network::NetworkProfile p;
      p.initializeFromString((char *)profile.c_str());
      uuids.push_back(id);
      profile_map[id] = p;
      receive_map[id] = std::map<ros::Time, uint64_t>();      
      printf("added uuid: %lu\n",id);
    }

    void add_sender(std::string profileName)
    {
      Network::NetworkProfile p;
      p.initializeFromFile(profileName.c_str());
      uuids.push_back(p.uuid);
      profile_map[p.uuid] = p;
      receive_map[p.uuid] = std::map<ros::Time, uint64_t>();
      printf("added uuid: %lu\n",p.uuid);
    }

    void set_duration(double dur) { duration = ros::Duration(dur); }
    void set_output_filename(std::string filename) { output_filename = filename; }
    void set_recv_done_callback(boost::function<void()> func) { callback_func = func; }
    void set_enable_sendback(bool sendback) { enable_sendback = sendback; }

    void update_sender_stream(uint64_t uuid, ros::Time t, uint64_t new_size)
    {
      uint64_t prevData = 0;
      if ( receive_map[uuid].size() )
	prevData = receive_map[uuid].end()->second;
      receive_map[uuid][t] = new_size + prevData;
    }

    ros::Time get_end_time() const { return ros::Time(endTime); }

    int init_oob()
    {
      sd = socket(AF_INET, SOCK_DGRAM, 0);
      if(sd < 0)
	{
	  perror("Opening datagram socket error");
	  exit(1);
	}
      else
	printf("Opening the datagram socket...OK.\n");

      /* Initialize the group sockaddr structure with a */
      /* group address of 225.1.1.1 and port 5555. */
      memset((char *) &groupSock, 0, sizeof(groupSock));
      groupSock.sin_family = AF_INET;
      groupSock.sin_addr.s_addr = inet_addr(oob_mc_group.c_str());
      groupSock.sin_port = htons(oob_mc_port);

      /* Set local interface for outbound multicast datagrams. */
      /* The IP address specified must be associated with a local, */
      /* multicast capable interface. */
      struct in_addr localInterface;
      localInterface.s_addr = inet_addr("0.0.0.0");
      if(setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0)
	{
	  perror("Setting local interface error");
	  exit(1);
	}
      else
	printf("Setting the local interface...OK\n");
    }

    int oob_send(std::vector<uint64_t>& send_uuids, bool val)
    {
      int num_disabled = send_uuids.size();
      if (!num_disabled)
	return 0;

      // FORMAT DATA
      char msg[(num_disabled+2) * 16];
      memset(msg,0,(num_disabled+2)*16);
      for (auto it = send_uuids.begin(); it != send_uuids.end(); ++it)
	{
	  sprintf(msg,"%s%lu,%d;", msg, *it, (int)val);
	}

      int len = sendto(sd, msg, strlen(msg)+1,
		       0, (struct sockaddr*)&groupSock, sizeof(groupSock));
      return 0;
    }

    void buffer_recv_threadfunc(void)
    {
      while ( true )
	{
	  // UPDATE SENDERS IF BUFFER IS CLEARING:
	  uint64_t currentSize = buffer.bits();
	  uint64_t currentCapacity = buffer.capacityBits();
	  if ( currentCapacity > 0 and enable_sendback )
	    {
	      double utilization = (double)currentSize/(double)currentCapacity;
	      ros::Time now = ros::Time::now();
	      if ( utilization < 0.75 )
		unlimit_ddos();
	      else if (utilization > 0.9)
		limit_ddos(now, 1.0);
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
	  if ( received_data and ros::Time::now() >= endTime )
	    {
	      if (this->callback_func)
		this->callback_func();
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

    void record()
    {
      printf("writing output\n");
      printf("max buffer: %lu\n", buffer.maxBits());
      printf("received msgs: %lu\n", messages.size());
      Network::write_data(output_filename.c_str(),messages);
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
    bool enable_sendback;

    uint64_t buffer_capacity_bits;
    
    int sd;
    struct sockaddr_in groupSock;

    boost::function<void()> callback_func;

    std::string output_filename;

    std::vector<uint64_t> uuids;
    std::vector<uint64_t> disabled_uuids;
    std::map<uint64_t, Network::NetworkProfile> profile_map;
    std::map<uint64_t, std::map<ros::Time, uint64_t>> receive_map; // uuid - > <time, data>
  };
};

#endif
