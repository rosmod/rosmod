#include "network/sender.hpp"

using namespace Network;

template<typename T>
sender<T>::sender()
{
  deactivated = false;

  // BIND TO MULTICAST SOCKET FOR RECEIVING OOB

  // CREATE THREAD FOR RECEIVING OOB COMMUNICATIONS
}

template<typename T>
void sender<T>::init(std::string profileName)
{
  profile.initializeFromFile(profileName.c_str());
  uuid = profile.uuid;
}

template<typename T>
void sender<T>::set_duration(double dur)
{
  duration = ros::Duration(dur);
  endTime = ros::Time::now() + duration;
}

template<typename T>
void sender<T>::set_output_filename(std::string filename)
{
  output_filename = filename;
}

template<typename T>
void sender<T>::send(ros::Publisher pub, const T& msg)
{
  // CHECK AGAINST PRIVATE VARIABLE : DEACTIVATED
  if (deactivated)
    throw Network::Exceeded_Production_Profile();
  // CHECK AGAINST PROFILE (INCLUDING METERING FROM RECEIVER)
  if (profile.Initialized()) // profile is not in error state
    {
      ros::Time now = ros::Time::now();
      double timeDiff = (now - nextSendTime).toSec();
      if (timeDiff < 0)
	{
	  throw Network::Exceeded_Production_Profile();
	}
      if (profile.resources.size() > 0) // has entries in profile
	{
	  msg.uuid = uuid;
	  uint64_t msgSizeBytes =
	    ros::serialization::Serializer<T>::serializedLength(msg);
	  timespec current_time;
	  current_time.tv_sec = now.sec;
	  current_time.tv_nsec = now.nsec;
	  timeDiff = profile.Delay(msgSizeBytes * 8, current_time);
	  nextSendTime = now + ros::Duration(timeDiff);
	}
    }
  // IF EVERYTHING IS ALRIGHT, PASS IT THROUGH
  pub.publish(msg);
}

template<typename T>
void sender<T>::oob_recv(void)
{
  // receive udp multicast message here
  // parse if for a list of UUIDs and their deactivated setting
  // if our uuid is in the list, set our deactivated variable accordingly
}
