#include "pub_sub_tg/sender.hpp"


//# Start User Globals Marker

void sender::message_pub_wrapper(const pub_sub_tg::message& msg)
{
  // CHECK AGAINST PRIVATE VARIABLE : DEACTIVATED
  if (deactivated)
    return;
  // CHECK AGAINST PROFILE (INCLUDING METERING FROM RECEIVER)
  if (profile.Initialized()) // profile is not in error state
    {
      ros::Time now = ros::Time::now();
      double timeDiff = (now - nextSendTime).toSec();
      if (timeDiff < 0)
	{
	  LOGGER.ERROR("ERROR: message_pub tried to exceed profile!");
	  throw Network::Exceeded_Production_Profile();
	}
      if (profile.resources.size() > 0) // has entries in profile
	{
	  uint64_t msgSizeBytes =
	    ros::serialization::Serializer<pub_sub_tg::message>::serializedLength(msg);
	  timespec current_time;
	  current_time.tv_sec = now.sec;
	  current_time.tv_nsec = now.nsec;
	  timeDiff = profile.Delay(msgSizeBytes * 8, current_time);
	  nextSendTime = now + ros::Duration(timeDiff);
	}
    }
  // IF EVERYTHING IS ALRIGHT, PASS IT THROUGH
  message_pub.publish(msg);
}

void sender::TrafficGeneratorTimer(const ros::TimerEvent& event)
{
  // AND A MEASUREMENT
  pub_sub_tg::message msg;
  msg.uuid = uuid;
  msg.bytes.resize(max_data_length,0);
  uint64_t msgSizeBytes =
    ros::serialization::Serializer<pub_sub_tg::message>::serializedLength(msg);

  Network::Message new_msg;
  new_msg.Bytes(msgSizeBytes);
  new_msg.Id(id++);
  new_msg.TimeStamp();
  messages.push_back(new_msg);

  try
    {
      message_pub_wrapper(msg);
    }
  catch ( Network::Exceeded_Production_Profile& ex )
    {
      LOGGER.DEBUG("Sender has been prevented from sending data for now.");
    }

  double timerDelay = profile.Delay(new_msg.Bits(), new_msg.LastEpochTime());

  if ( ros::Time::now() >= endTime )
    {
      printf("WRITING LOG\n");
      std::string fName = nodeName + "." + compName + ".network.csv";
      Network::write_data(fName.c_str(),messages);
    }
  else
    {
      ros::TimerOptions timer_options;
      timer_options = 
	ros::TimerOptions
	(ros::Duration(timerDelay),
	 boost::bind(&sender::TrafficGeneratorTimer, this, _1),
	 &this->compQueue,
	 true);
      ros::NodeHandle nh;
      tgTimer = nh.createTimer(timer_options);
    }
}

//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void sender::Init(const ros::TimerEvent& event)
{
  LOGGER.DEBUG("Entering sender::Init");
  // Initialize Here
  
  // INITIALIZE N/W MIDDLEWARE HERE
  printf("Initializing MW\n");
  ros::Time now = ros::Time::now();
  endTime = now + ros::Duration(config.tg_time);
  // NEED TO GET UUID & NETWORK PROFILE FROM XML
  metered = false;
  deactivated = false;
  uuid = this->config.oob_uuid;
  max_data_length = 8192;
  // LOAD NETWORK PROFILE HERE
  profileName = this->config.profileName;
  profile.initializeFromFile(profileName.c_str());
  printf("Initialized Profile\n");
  printf("%s\n",profile.toString().c_str());
  // FINISH NETWORK MIDDLEWARE INIT
  id = 0;

  // CREATE TIMER HERE FOR SENDING DATA ACCORDING TO PROFILE
  ros::NodeHandle nh;
  ros::TimerOptions timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&sender::TrafficGeneratorTimer, this, _1),
     &this->compQueue,
     true);
  tgTimer = nh.createTimer(timer_options);
  printf("Created Timer\n");

  // Stop Init Timer
  initOneShotTimer.stop();
  LOGGER.DEBUG("Exiting sender::Init");
}
//# End Init Marker

// Server Callback - oob_server
//# Start oob_commCallback Marker
bool sender::oob_commCallback(pub_sub_tg::oob_comm::Request  &req,
  pub_sub_tg::oob_comm::Response &res)
{
  LOGGER.DEBUG("Entering sender::oob_commCallback");
  // Business Logic for oob_server Server

  deactivated = req.deactivateSender;

  res.uuid = uuid;
  res.profileName = profileName;
  res.retVal = 0;
  
  LOGGER.DEBUG("Exiting sender::oob_commCallback");
  return true;
}
//# End oob_commCallback Marker


// Destructor - Cleanup Ports & Timers
sender::~sender()
{
  message_pub.shutdown();
  oob_server.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void sender::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Scheduling Scheme is FIFO

  // Component Publisher - message_pub
  advertiseName = "message";
  if (portGroupMap.find("message_pub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["message_pub"];
  this->message_pub = nh.advertise<pub_sub_tg::message>(advertiseName.c_str(), 1000);

  // Component Server - oob_server
  advertiseName = "oob_comm";
  if (portGroupMap.find("oob_server") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["oob_server"];
  ros::AdvertiseServiceOptions oob_server_server_options;
  oob_server_server_options = ros::AdvertiseServiceOptions::create<pub_sub_tg::oob_comm>
      (advertiseName.c_str(),
       boost::bind(&sender::oob_commCallback, this, _1, _2),
       ros::VoidPtr(),
       &this->compQueue);
  this->oob_server = nh.advertiseService(oob_server_server_options);  
 
  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&sender::Init, this, _1),
     &this->compQueue,
     true,
     false);
  this->initOneShotTimer = nh.createTimer(timer_options);
  this->initOneShotTimer.stop();
  // Identify the pwd of Node Executable
  std::string s = node_argv[0];
  std::string exec_path = s;
  std::string delimiter = "/";
  std::string exec, pwd;
  size_t pos = 0;
  while ((pos = s.find(delimiter)) != std::string::npos) {
    exec = s.substr(0, pos);
    s.erase(0, pos + delimiter.length());
  }
  exec = s.substr(0, pos);
  pwd = exec_path.erase(exec_path.find(exec), exec.length());
  std::string log_file_path = pwd + nodeName + "." + compName + ".log"; 
  
  // Create the log file & open file stream
  LOGGER.CREATE_FILE(log_file_path);
  
  // Establish log levels of LOGGER
  LOGGER.SET_LOG_LEVELS(logLevels);


  this->comp_sync_pub = nh.advertise<std_msgs::Bool>("component_synchronization", 1000);
  
  ros::SubscribeOptions comp_sync_sub_options;
  comp_sync_sub_options = ros::SubscribeOptions::create<std_msgs::Bool>
    ("component_synchronization",
     1000,
     boost::bind(&sender::component_synchronization_OnOneData, this, _1),
     ros::VoidPtr(),
     &this->compQueue);
  this->comp_sync_sub = nh.subscribe(comp_sync_sub_options);

  ros::Time now = ros::Time::now();
  while ( this->comp_sync_sub.getNumPublishers() < this->num_comps_to_sync &&
	  (ros::Time::now() - now) < ros::Duration(comp_sync_timeout) )
    ros::Duration(0.1).sleep();
  ros::Duration(0.5).sleep();
  this->comp_sync_sub.shutdown();
  this->comp_sync_pub.shutdown();

  this->initOneShotTimer.start();
  
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new sender(config,argc,argv);
  }
}
