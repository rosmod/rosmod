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

  // create the multicast receive socket
  if ( (oob_mc_recv_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
      perror("socket");
      return -1;
    }
  memset(&oob_mc_recv_sockaddr, 0, sizeof(oob_mc_recv_sockaddr));
  oob_mc_recv_sockaddr.sin_family = AF_INET;
  oob_mc_recv_sockaddr.sin_port = htons(oob_mc_port);
  oob_mc_recv_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  // bind to the addrress
  if ( bind( oob_mc_recv_sockfd,
	     (struct sockaddr *)&oob_mc_recv_sockaddr,
	     sizeof(oob_mc_recv_sockaddr)) )
    {
      perror("binding");
      return -1;
    }
  // joine the multicast group
  struct ip_mreq group;
  group.imr_multiaddr.s_addr = inet_addr(oob_mc_group.c_str());
  group.imr_interface.s_addr = INADDR_ANY;
  if ( setsockopt( oob_mc_recv_sockfd,
		   IPPROTO_IP,
		   IP_ADD_MEMBERSHIP,
		   (char *)&group,
		   sizeof(group) ) < 0 )
    {
      perror("multicast group");
      return -1;
    }
  // set the receive timeout
  struct timespec tv;
  tv.tv_sec = 1;
  tv.tv_nsec = 0;
  if ( setsockopt( oob_mc_recv_sockfd,
		   SOL_SOCKET,
		   SO_RCVTIMEO,
		   &tv,
		   sizeof(tv) ) < 0 )
    {
      perror("timeout setting");
      return -1;
    }
  // CREATE THREAD FOR RECEIVING OOB COMMUNICATIONS
  boost::thread oob_recv_thread = boost::thread(&sender::oob_recv_threadfunc, this);
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

void sender::oob_recv_threadfunc(void)
{
  while (1)
    {
      // receive udp multicast message here
      int status;
      char buffer[max_recv_buffer_size];

      struct sockaddr_in saddr;
      int socklen;
      memset(&saddr, 0, sizeof(saddr));

      status = recvfrom( oob_mc_recv_sockfd,
			 buffer,
			 max_recv_buffer_size,
			 0,
			 (struct sockaddr *)&saddr,
			 (socklen_t *)&socklen );
      if ( status > 0 )
	{
	  bool has_uuid = false;
	  bool my_val = false;
	  // parse if for a list of UUIDs and their deactivated setting
	  std::string s = buffer;
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
    }
}
