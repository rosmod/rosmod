#include "pub_sub_tg/sender.hpp"


//# Start User Globals Marker
void sender::TrafficGeneratorTimer(const ros::TimerEvent& event)
{
  // AND A MEASUREMENT
  pub_sub_tg::message msg;
  msg.uuid = sender_middleware.uuid;
  msg.bytes.resize(max_data_length,0);
  uint64_t msgSizeBytes =
    ros::serialization::Serializer<pub_sub_tg::message>::serializedLength(msg);

  Network::Message new_msg;
  new_msg.Bytes(msgSizeBytes);
  new_msg.Id(id);
  new_msg.TimeStamp();

  double timerDelay = 0;

  try
    {
      sender_middleware.send(message_pub, msg);
      sender_middleware.messages.push_back(new_msg);
      timerDelay = sender_middleware.profile.Delay(new_msg.Bits(), new_msg.LastEpochTime());
      id++;
    }
  catch ( Network::Exceeded_Production_Profile& ex )
    {
    }

  if ( ros::Time::now() >= sender_middleware.endTime )
    {
      LOGGER.DEBUG("WRITING LOG, sent %lu messages",
		   sender_middleware.messages.size());
      std::string fName = nodeName + "." + compName + ".network.csv";
      Network::write_data(fName.c_str(), sender_middleware.messages);
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
  LOGGER.DEBUG("Initializing MW");

  double tg_duration = -1;

  max_data_length = 8192;

  for (int i=0; i<node_argc; i++)
    {
      if (!strcmp(node_argv[i], "--max_data_length_bytes"))
	{
	  max_data_length = atoi(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i], "--max_data_length_bits"))
	{
	  max_data_length = atoi(node_argv[i+1]) / 8;
	}
      if (!strcmp(node_argv[i], "--tg_time"))
	{
	  tg_duration = atof(node_argv[i+1]);
	}
    }

  LOGGER.DEBUG("Using data length of: %lu",max_data_length);

  // LOAD NETWORK PROFILE HERE
  profileName = this->config.profileName;
  sender_middleware.profile.initializeFromFile(profileName.c_str());
  LOGGER.DEBUG("Initialized Profile");
  LOGGER.DEBUG("%s",sender_middleware.profile.toString().c_str());

  // FINISH NETWORK MIDDLEWARE INIT
  if (tg_duration < 0)
    tg_duration = sender_middleware.profile.period;
  sender_middleware.set_duration(tg_duration);

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
  LOGGER.DEBUG("Created Timer");

  // Stop Init Timer
  initOneShotTimer.stop();
  LOGGER.DEBUG("Exiting sender::Init");
}
//# End Init Marker

// Destructor - Cleanup Ports & Timers
sender::~sender()
{
  message_pub.shutdown();
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
