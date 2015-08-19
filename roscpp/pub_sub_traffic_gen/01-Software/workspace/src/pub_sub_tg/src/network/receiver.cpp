#include "network/receiver.hpp"

using namespace Network;

receiver::receiver()
{
  received_data = false;

  ros::NodeHandle nh;
  oob_pub = nh.advertise<std_msgs::UInt64MultiArray>("oob_control", 1000);

  // CREATE THREAD HERE FOR RECEIVING DATA
  boost::thread *tmr_thread =
    new boost::thread( boost::bind(&receiver::buffer_receive_thread, this) );
}

void receiver::set_duration(double dur)
{
  duration = ros::Duration(dur);
}

void receiver::set_output_filename(std::string filename)
{
  output_filename = filename;
}

void receiver::add_sender(std::string profileName)
{
  Network::NetworkProfile p;
  p.initializeFromFile(profileName.c_str());

  uuids.push_back(p.uuid);
  profile_map[p.uuid] = p;
  receive_map[p.uuid] = std::map<ros::Time, uint64_t>();
}

void receiver::update_sender_stream(uint64_t uuid, ros::Time t, uint64_t new_size)
{
  uint64_t prevData = 0;
  if ( receive_map[uuid].size() )
    prevData = receive_map[uuid].end()->second;
  receive_map[uuid][t] = new_size + prevData;
}

void receiver::buffer_receive_thread(void)
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

void receiver::oob_send(std::vector<uint64_t>& send_uuids, bool val)
{
  int num_disabled = send_uuids.size();

  std_msgs::UInt64MultiArray oob_message;
    
  oob_message.layout.dim[0].label = "uuid";
  oob_message.layout.dim[0].size = 1;
  oob_message.layout.dim[0].stride = 2;
  oob_message.layout.dim[1].label = "enabled";
  oob_message.layout.dim[1].size = 1;
  oob_message.layout.dim[1].stride = 1;
  oob_message.layout.data_offset = 0;
  
  int index = 0;
  for (auto uuid_it = send_uuids.begin();
       uuid_it != send_uuids.end(); ++uuid_it)
    {
      oob_message.data[index*2] = *uuid_it;
      oob_message.data[index*2 + 1] = uint64_t(val);
    }
  oob_pub.publish(oob_message);
}

void receiver::unlimit_ddos(void)
{
  oob_send(disabled_uuids, false);
  disabled_uuids.clear();
}

void receiver::limit_ddos(ros::Time now, double timeWindow)
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
  oob_send(bad_uuids, true);
}
