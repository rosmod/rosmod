#include "pkg/receiver.hpp"


//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void receiver::Init(const ros::TimerEvent& event)
{
  // Initialize Here
  srand (time(NULL));
  double tg_duration = -1;
  std::string fName;
  for (int i=0; i<node_argc; i++)
    {
      if (!strcmp(node_argv[i], "--tg_time"))
	{
	  tg_duration = atof(node_argv[i+1]);
	}
    }
  uint64_t capacityBits = 400000;
  bool enable_sendback = true;
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
      if (!strcmp(node_argv[i], "--disable_sendback"))
	{
	  enable_sendback = false;
	}
    }
  if (config.profileMap.find("routed_msg_sub") != config.profileMap.end())
    {
      routed_msg_sub_recv_mw.init(node_argc,
					     node_argv,
					     config.profileMap["routed_msg_sub"],
					     capacityBits);
      if ( tg_duration < 0 )
	routed_msg_sub_recv_mw.set_duration(routed_msg_sub_recv_mw.profile.period);
      else
	routed_msg_sub_recv_mw.set_duration(tg_duration);
      fName = nodeName + "." + compName + ".routed_msg_sub.network.csv";
      routed_msg_sub_recv_mw.set_enable_sendback(enable_sendback);
      routed_msg_sub_recv_mw.set_output_filename(fName);
      routed_msg_sub_recv_mw.set_recv_done_callback(boost::bind(&receiver::mw_recv_done_callback, this, &routed_msg_sub_recv_mw));
      routed_msg_sub_id = 0;
    }
  if (config.portSenderMap.find("routed_msg_sub") != config.portSenderMap.end())
    {
      for (auto it=config.portSenderMap["routed_msg_sub"].begin();
	   it != config.portSenderMap["routed_msg_sub"].end(); ++it)
	{
	  routed_msg_sub_recv_mw.add_sender( it->first, it->second );
	}
    }
  
  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker


void receiver::mw_recv_done_callback(Network::receiver* receiver_mw)
{
  LOGGER.DEBUG("Writing middleware log.");
  LOGGER.DEBUG("Max middleware buffer: %lu bits", receiver_mw->buffer.maxBits());
  receiver_mw->record();
}

// Subscriber Callback - routed_msg_sub
//# Start routed_msg_sub_OnOneData Marker
void receiver::routed_msg_sub_OnOneData(const pkg::routed_msg::ConstPtr& received_data)
{
  // Business Logic for routed_msg_sub Subscriber

  uint64_t uuid = received_data->uuid;
  uint64_t msgBytes = ros::serialization::Serializer<pkg::routed_msg>::serializedLength(*received_data);
  ros::Time now = ros::Time::now();
  routed_msg_sub_recv_mw.update_sender_stream(uuid, now, msgBytes * 8);
  Network::Message new_msg;
  new_msg.Bytes(msgBytes);
  new_msg.Id(routed_msg_sub_id++);
  new_msg.TimeStamp();
  routed_msg_sub_recv_mw.buffer.send(new_msg, msgBytes * 8);
  
}
//# End routed_msg_sub_OnOneData Marker


// Destructor - Cleanup Ports & Timers
receiver::~receiver()
{
  routed_msg_sub.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void receiver::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

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

  // Scheduling Scheme is FIFO

  // Component Subscriber - routed_msg_sub
  advertiseName = "routed_msg";
  if (portGroupMap.find("routed_msg_sub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["routed_msg_sub"];
  ros::SubscribeOptions routed_msg_sub_options;
  routed_msg_sub_options = ros::SubscribeOptions::create<pkg::routed_msg>
      (advertiseName.c_str(),
       1000,
       boost::bind(&receiver::routed_msg_sub_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->routed_msg_sub = nh.subscribe(routed_msg_sub_options);  

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
  ros::Duration(0.5).sleep();
  this->comp_sync_sub.shutdown();
  this->comp_sync_pub.shutdown();

  this->initOneShotTimer.start();
  
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new receiver(config,argc,argv);
  }
}
