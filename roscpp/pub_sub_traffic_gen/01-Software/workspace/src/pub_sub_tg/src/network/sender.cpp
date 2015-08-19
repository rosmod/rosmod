#include "network/sender.hpp"

using namespace Network;

sender::sender()
{
  deactivated = false;
  id = 0;
}

void sender::init(std::string profileName)
{
  profile.initializeFromFile(profileName.c_str());
  uuid = profile.uuid;

  // BIND TO MULTICAST SOCKET FOR RECEIVING OOB
  // CREATE THREAD FOR RECEIVING OOB COMMUNICATIONS
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
