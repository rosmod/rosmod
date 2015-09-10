#include "dsc/tl_actuator.hpp"


//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void tl_actuator::Init(const ros::TimerEvent& event)
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
  if (config.profileMap.find("ryg_control_sub") != config.profileMap.end())
    {
      ryg_control_sub_recv_mw.init(node_argc,
					     node_argv,
					     config.profileMap["ryg_control_sub"],
					     capacityBits);
      if ( tg_duration < 0 )
	ryg_control_sub_recv_mw.set_duration(ryg_control_sub_recv_mw.profile.period);
      else
	ryg_control_sub_recv_mw.set_duration(tg_duration);
      fName = nodeName + "." + compName + ".ryg_control_sub.network.csv";
      ryg_control_sub_recv_mw.set_enable_sendback(enable_sendback);
      ryg_control_sub_recv_mw.set_output_filename(fName);
      ryg_control_sub_recv_mw.set_recv_done_callback(boost::bind(&tl_actuator::mw_recv_done_callback, this, &ryg_control_sub_recv_mw));
      ryg_control_sub_id = 0;
    }
  if (config.portSenderMap.find("ryg_control_sub") != config.portSenderMap.end())
    {
      for (auto it=config.portSenderMap["ryg_control_sub"].begin();
	   it != config.portSenderMap["ryg_control_sub"].end(); ++it)
	{
	  ryg_control_sub_recv_mw.add_sender( it->first, it->second );
	}
    }
  
  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker




// Subscriber Callback - ryg_control_sub
//# Start ryg_control_sub_OnOneData Marker
void tl_actuator::ryg_control_sub_OnOneData(const dsc::ryg_control::ConstPtr& received_data)
{
  // Business Logic for ryg_control_sub Subscriber

  uint64_t uuid = received_data->uuid;
  uint64_t msgBytes = ros::serialization::Serializer<dsc::ryg_control>::serializedLength(*received_data);
  ros::Time now = ros::Time::now();
  ryg_control_sub_recv_mw.update_sender_stream(uuid, now, msgBytes * 8);
  Network::Message new_msg;
  new_msg.Bytes(msgBytes);
  new_msg.Id(ryg_control_sub_id++);
  new_msg.TimeStamp();
  ryg_control_sub_recv_mw.buffer.send(new_msg, msgBytes * 8);
  
}
//# End ryg_control_sub_OnOneData Marker


// Destructor - Cleanup Ports & Timers
tl_actuator::~tl_actuator()
{
  ryg_control_sub.shutdown();
  tlc_set_ryg_state_client.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void tl_actuator::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Scheduling Scheme is FIFO

  // Component Subscriber - ryg_control_sub
  advertiseName = "ryg_control";
  if (portGroupMap.find("ryg_control_sub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["ryg_control_sub"];
  ros::SubscribeOptions ryg_control_sub_options;
  ryg_control_sub_options = ros::SubscribeOptions::create<dsc::ryg_control>
      (advertiseName.c_str(),
       1000,
       boost::bind(&tl_actuator::ryg_control_sub_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->ryg_control_sub = nh.subscribe(ryg_control_sub_options);  

  // Configure all required services associated with this component
  // Component Client - tlc_set_ryg_state_client
  advertiseName = "sumo_tlc_set_ryg_state";
  if (portGroupMap.find("tlc_set_ryg_state_client") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["tlc_set_ryg_state_client"];
  this->tlc_set_ryg_state_client = nh.serviceClient<dsc::sumo_tlc_set_ryg_state>(advertiseName.c_str(), true); 

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&tl_actuator::Init, this, _1),
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


  this->Comp_Sync_Pub = Nh.Advertise<Std_Msgs::Bool>("Component_Synchronization", 1000);
  
  ros::Subscribeoptions Comp_Sync_Sub_Options;
  comp_sync_sub_options = ros::SubscribeOptions::create<std_msgs::Bool>
    ("component_synchronization",
     1000,
     boost::bind(&tl_actuator::component_synchronization_OnOneData, this, _1),
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
    return new tl_actuator(config,argc,argv);
  }
}
