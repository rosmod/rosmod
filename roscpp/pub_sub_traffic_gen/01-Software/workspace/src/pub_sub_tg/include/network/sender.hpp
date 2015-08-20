#ifndef NETWORK_SENDER_HPP
#define NETWORK_SENDER_HPP

#include "ros/ros.h"

#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "network/NetworkProfile.hpp"

namespace Network
{
  static const std::string oob_mc_group = "224.0.0.251";
  static const int oob_mc_port = 12345;
  
  class Exceeded_Production_profile {}; // sender-side exception

  class sender
  {
  public:
    sender()
      : socket_(io_service_)
    {
      deactivated = false;
      id = 0;
      // create the multicast receive socket
      boost::asio::ip::address
	listen_address = boost::asio::ip::address::from_string("0.0.0.0");
      boost::asio::ip::udp::endpoint
	listen_endpoint(
			listen_address,
			oob_mc_port);
      socket_.open(listen_endpoint.protocol());
      socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
      socket_.bind(listen_endpoint);

      // Join the multicast group.
      boost::asio::ip::address
	multicast_address = boost::asio::ip::address::from_string(oob_mc_group);
      socket_.set_option(
			 boost::asio::ip::multicast::join_group(multicast_address));

      boost::thread *io_thread =
	new boost::thread( boost::bind(&sender::oob_recv_threadfunc, this) );
    }

    int init(std::string profileName)
    {
      profile.initializeFromFile(profileName.c_str());
      uuid = profile.uuid;
      return 0;
    }

    void set_duration(double dur) { endTime = ros::Time::now() + ros::Duration(dur); }
    void set_output_filename(std::string filename) { output_filename = filename; }

    uint64_t get_uuid() const { return uuid; }
    ros::Time get_end_time() const { return ros::Time(endTime); }

    void record() { Network::write_data(output_filename.c_str(), messages); }

    template <typename T>
    double send(ros::Publisher pub, const T& msg)
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

    void oob_recv_threadfunc()
    {
      while (1)
	{
	  boost::asio::socket_base::non_blocking_io command(true);
	  //socket_.io_control(command);
	  printf("going into receive_from\n");
                       
	  boost::asio::ip::udp::endpoint sender_endpoint;
	  size_t length = socket_.receive_from(
					       boost::asio::buffer(data_,
								   max_recv_buffer_size),
					       sender_endpoint);
	  printf("coming out of receive_from\n");
	  if (length > 0)
	    {
	      printf("got message!\n");
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
		  printf("got val: %d\n",my_val);
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
    uint64_t id;

    static const int max_recv_buffer_size = 2000;
    boost::asio::io_service io_service_;
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint endpoint_;
    char data_[max_recv_buffer_size];


    std::string output_filename;
  };
};

#endif
