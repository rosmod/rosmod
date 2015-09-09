#include "dsc/controller.hpp"


//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void controller::Init(const ros::TimerEvent& event)
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
  if (config.profileMap.find("sensor_state_sub") != config.profileMap.end())
    {
      sensor_state_sub_recv_mw.init(node_argc,
					     node_argv,
					     config.profileMap["sensor_state_sub"],
					     capacityBits);
      if ( tg_duration < 0 )
	sensor_state_sub_recv_mw.set_duration(sensor_state_sub_recv_mw.profile.period);
      else
	sensor_state_sub_recv_mw.set_duration(tg_duration);
      fName = nodeName + "." + compName + ".sensor_state_sub.network.csv";
      sensor_state_sub_recv_mw.set_enable_sendback(enable_sendback);
      sensor_state_sub_recv_mw.set_output_filename(fName);
      sensor_state_sub_recv_mw.set_recv_done_callback(boost::bind(&controller::mw_recv_done_callback, this, &sensor_state_sub_recv_mw));
      sensor_state_sub_id = 0;
    }
  if (config.portSenderMap.find("sensor_state_sub") != config.portSenderMap.end())
    {
      for (auto it=config.portSenderMap["sensor_state_sub"].begin();
	   it != config.portSenderMap["sensor_state_sub"].end(); ++it)
	{
	  sensor_state_sub_recv_mw.add_sender( it->first, it->second );
	}
    }
  
  if (config.profileMap.find("ryg_state_sub") != config.profileMap.end())
    {
      ryg_state_sub_recv_mw.init(node_argc,
					     node_argv,
					     config.profileMap["ryg_state_sub"],
					     capacityBits);
      if ( tg_duration < 0 )
	ryg_state_sub_recv_mw.set_duration(ryg_state_sub_recv_mw.profile.period);
      else
	ryg_state_sub_recv_mw.set_duration(tg_duration);
      fName = nodeName + "." + compName + ".ryg_state_sub.network.csv";
      ryg_state_sub_recv_mw.set_enable_sendback(enable_sendback);
      ryg_state_sub_recv_mw.set_output_filename(fName);
      ryg_state_sub_recv_mw.set_recv_done_callback(boost::bind(&controller::mw_recv_done_callback, this, &ryg_state_sub_recv_mw));
      ryg_state_sub_id = 0;
    }
  if (config.portSenderMap.find("ryg_state_sub") != config.portSenderMap.end())
    {
      for (auto it=config.portSenderMap["ryg_state_sub"].begin();
	   it != config.portSenderMap["ryg_state_sub"].end(); ++it)
	{
	  ryg_state_sub_recv_mw.add_sender( it->first, it->second );
	}
    }
  
  max_data_length = 8192;
  tg_misbehave = false;
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
      if (!strcmp(node_argv[i], "--tg_misbehave"))
	{
	  tg_misbehave = true;
	}
    }
  ros::NodeHandle nh;
  ros::TimerOptions timer_options;
  if (config.profileMap.find("ryg_control_pub") != config.profileMap.end())
    {
      ryg_control_pub_send_mw.init(node_argc,
					     node_argv,
					     config.uuidMap["ryg_control_pub"],
					     config.profileMap["ryg_control_pub"]);
      if ( tg_duration < 0 )
	ryg_control_pub_send_mw.set_duration(ryg_control_pub_send_mw.profile.period);
      else
	ryg_control_pub_send_mw.set_duration(tg_duration);
      fName = nodeName + "." + compName + ".ryg_control_pub.network.csv";
      ryg_control_pub_send_mw.set_output_filename(fName);

      timer_options = 
	ros::TimerOptions
	(ros::Duration(-1),
	 boost::bind(&controller::ryg_control_pub_timerCallback, this, _1),
	 &this->compQueue,
	 true);
      ryg_control_pub_timer = nh.createTimer(timer_options);
    }
  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker


void controller::ryg_control_pub_timerCallback(const ros::TimerEvent& event)
{
  dsc::ryg_control msg;
  msg.uuid = ryg_control_pub_send_mw.get_uuid();
  msg.bytes.resize(max_data_length,0);
  double timerDelay = 0;
  try
    {
      timerDelay =
	ryg_control_pub_send_mw.send<dsc::ryg_control>(ryg_control_pub, msg);
    }
  catch ( Network::Exceeded_Production_Profile& ex )
    {
      LOGGER.DEBUG("Prevented from sending on the network!");
    }

  if ( ros::Time::now() >= ryg_control_pub_send_mw.get_end_time() )
    {
      LOGGER.DEBUG("writing output\n");
      ryg_control_pub_send_mw.record();
    }
  else
    {
      if (tg_misbehave)
	timerDelay -= 0.1;
      ros::TimerOptions timer_options;
      timer_options = 
	ros::TimerOptions
	(ros::Duration(timerDelay),
	 boost::bind(&controller::ryg_control_pub_timerCallback, this, _1),
	 &this->compQueue,
	 true);
      ros::NodeHandle nh;
      ryg_control_pub_timer = nh.createTimer(timer_options);
    }
}

void controller::mw_recv_done_callback(Network::receiver* receiver_mw)
{
  LOGGER.DEBUG("Writing middleware log.");
  LOGGER.DEBUG("Max middleware buffer: %lu bits", receiver_mw->buffer.maxBits());
  receiver_mw->record();
}

// Subscriber Callback - sensor_state_sub
//# Start sensor_state_sub_OnOneData Marker
void controller::sensor_state_sub_OnOneData(const dsc::sensor_state::ConstPtr& received_data)
{
  // Business Logic for sensor_state_sub Subscriber

  uint64_t uuid = received_data->uuid;
  uint64_t msgBytes = ros::serialization::Serializer<dsc::sensor_state>::serializedLength(*received_data);
  ros::Time now = ros::Time::now();
  sensor_state_sub_recv_mw.update_sender_stream(uuid, now, msgBytes * 8);
  Network::Message new_msg;
  new_msg.Bytes(msgBytes);
  new_msg.Id(sensor_state_sub_id++);
  new_msg.TimeStamp();
  sensor_state_sub_recv_mw.buffer.send(new_msg, msgBytes * 8);
  
}
//# End sensor_state_sub_OnOneData Marker
// Subscriber Callback - ryg_state_sub
//# Start ryg_state_sub_OnOneData Marker
void controller::ryg_state_sub_OnOneData(const dsc::ryg_state::ConstPtr& received_data)
{
  // Business Logic for ryg_state_sub Subscriber

  uint64_t uuid = received_data->uuid;
  uint64_t msgBytes = ros::serialization::Serializer<dsc::ryg_state>::serializedLength(*received_data);
  ros::Time now = ros::Time::now();
  ryg_state_sub_recv_mw.update_sender_stream(uuid, now, msgBytes * 8);
  Network::Message new_msg;
  new_msg.Bytes(msgBytes);
  new_msg.Id(ryg_state_sub_id++);
  new_msg.TimeStamp();
  ryg_state_sub_recv_mw.buffer.send(new_msg, msgBytes * 8);
  
}
//# End ryg_state_sub_OnOneData Marker

// Timer Callback - controller_timer
//# Start controller_timerCallback Marker
void controller::controller_timerCallback(const ros::TimerEvent& event)
{
  // Business Logic for controller_timer Timer

}
//# End controller_timerCallback Marker


// Destructor - Cleanup Ports & Timers
controller::~controller()
{
  controller_timer.stop();
  ryg_control_pub.shutdown();
  sensor_state_sub.shutdown();
  ryg_state_sub.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void controller::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Scheduling Scheme is FIFO

  // Component Subscriber - sensor_state_sub
  advertiseName = "sensor_state";
  if (portGroupMap.find("sensor_state_sub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["sensor_state_sub"];
  ros::SubscribeOptions sensor_state_sub_options;
  sensor_state_sub_options = ros::SubscribeOptions::create<dsc::sensor_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller::sensor_state_sub_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->sensor_state_sub = nh.subscribe(sensor_state_sub_options);  
  // Component Subscriber - ryg_state_sub
  advertiseName = "ryg_state";
  if (portGroupMap.find("ryg_state_sub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["ryg_state_sub"];
  ros::SubscribeOptions ryg_state_sub_options;
  ryg_state_sub_options = ros::SubscribeOptions::create<dsc::ryg_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller::ryg_state_sub_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->ryg_state_sub = nh.subscribe(ryg_state_sub_options);  

  // Component Publisher - ryg_control_pub
  advertiseName = "ryg_control";
  if (portGroupMap.find("ryg_control_pub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["ryg_control_pub"];
  this->ryg_control_pub = nh.advertise<dsc::ryg_control>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&controller::Init, this, _1),
     &this->compQueue,
     true,
     false);
  this->initOneShotTimer = nh.createTimer(timer_options);
  this->initOneShotTimer.stop();
  // Component Timer - controller_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.1),
     boost::bind(&controller::controller_timerCallback, this, _1),
     &this->compQueue, false, false);
  this->controller_timer = nh.createTimer(timer_options);

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
     boost::bind(&controller::component_synchronization_OnOneData, this, _1),
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
  this->controller_timer.start();
  
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new controller(config,argc,argv);
  }
}
