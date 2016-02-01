#include "pub_sub_tg/receiver.hpp"


//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void receiver::Init(const ros::TimerEvent& event)
{
  LOGGER.DEBUG("Entering receiver::Init");
  // Initialize Here

  double tg_duration = -1;
  uint64_t capacityBits = 400000;

  for (int i=0; i<node_argc; i++)
    {
      if (!strcmp(node_argv[i], "--buffer_capacity_bits"))
	{
	  capacityBits = atoi(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i], "--buffer_capacity_bytes"))
	{
	  capacityBits = atoi(node_argv[i+1]) * 8;
	}
      if (!strcmp(node_argv[i], "--tg_time"))
	{
	  tg_duration = atof(node_argv[i+1]);
	}
    }
  
  LOGGER.DEBUG("Initializing MW");
  printf("init mw\n");
  
  if (config.profileMap.find("message_sub") != config.profileMap.end())
    {
      receiver_middleware.init(node_argc,
			       node_argv,
			       config.profileMap["message_pub"],
			       capacityBits);

      // set up uuids for senders
      for (auto it = config.portSenderMap["message_sub"].begin();
	   it != config.portSenderMap["message_sub"].end(); ++it)
	{
	  receiver_middleware.add_sender( it->first, it->second );
	}
    }

  // done initializing receiver middleware

  id = 0;

  // Stop Init Timer
  initOneShotTimer.stop();
  LOGGER.DEBUG("Exiting receiver::Init");
}
//# End Init Marker

// Subscriber Callback - message_sub
//# Start message_sub_OnOneData Marker
void receiver::message_sub_OnOneData(const pub_sub_tg::message::ConstPtr& received_data)
{
  // GET SENDER ID
  uint64_t uuid = received_data->uuid;

  // MEASURE SIZE OF INCOMING MESSAGE
  uint64_t msgBytes =
    ros::serialization::Serializer<pub_sub_tg::message>::serializedLength(*received_data);

  // KEEP TRACK OF EACH SENDER'S INCOMING DATA PROFILE
  ros::Time now = ros::Time::now();
  receiver_middleware.update_sender_stream(uuid, now, msgBytes * 8);

  // MANAGE AVAILABLE BUFFER SPACE
  uint64_t currentSize = receiver_middleware.buffer.bits();
  uint64_t currentCapacity = receiver_middleware.buffer.capacityBits();
  if ( currentCapacity > 0 )
    {
      double utilization = (double)currentSize/(double)currentCapacity;
      if (utilization > 0.95)
	{
	  receiver_middleware.limit_ddos(now, 1.0); // FOR NOW ONLY LOOKING AT PREVIOUS SECOND
	}
    }
  
  // RECORD MESSAGE
  Network::Message new_msg;
  new_msg.Bytes(msgBytes);
  new_msg.Id(id++);
  new_msg.TimeStamp();

  // PUT MESSAGE INTO A BUFFER
  receiver_middleware.buffer.send(new_msg, msgBytes * 8);
}
//# End message_sub_OnOneData Marker


// Destructor - Cleanup Ports & Timers
receiver::~receiver()
{
  message_sub.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void receiver::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Scheduling Scheme is FIFO

  // Component Subscriber - message_sub
  advertiseName = "message";
  if (portGroupMap.find("message_sub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["message_sub"];
  ros::SubscribeOptions message_sub_options;
  message_sub_options = ros::SubscribeOptions::create<pub_sub_tg::message>
      (advertiseName.c_str(),
       1000,
       boost::bind(&receiver::message_sub_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  message_sub_options.transport_hints = ros::TransportHints()
    .udp();

  this->message_sub = nh.subscribe(message_sub_options);  

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&receiver::Init, this, _1),
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
     boost::bind(&receiver::component_synchronization_OnOneData, this, _1),
     ros::VoidPtr(),
     &this->compQueue);
  this->comp_sync_sub = nh.subscribe(comp_sync_sub_options);

  ros::Time now = ros::Time::now();
  while ( this->comp_sync_sub.getNumPublishers() < this->num_comps_to_sync &&
	  (ros::Time::now() - now) < ros::Duration(comp_sync_timeout) )
    ros::Duration(0.1).sleep();
  this->comp_sync_sub.shutdown();
  this->comp_sync_pub.shutdown();

  this->initOneShotTimer.start();
  
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new receiver(config,argc,argv);
  }
}
