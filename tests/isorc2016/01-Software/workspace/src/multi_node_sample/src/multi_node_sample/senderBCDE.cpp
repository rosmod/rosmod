#include "multi_node_sample/senderBCDE.hpp"

//# Start User Globals Marker
double multiplier = 0.3;
double _profile_period = 9.0;
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void senderBCDE::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering senderBCDE::init_timer_operation");
#endif
  // Initialize Here
#if 0
  for (int i=0; i<node_argc; i++)
    {
      if (!strcmp(node_argv[i], "--multiplier"))
	{
	  multiplier = atof(node_argv[i+1]);
	}
    }

  srand (time(NULL));
  double tg_duration = -1;
  std::string fName;
  bool enable_oob = false;
  for (int i=0; i<node_argc; i++)
    {
      if (!strcmp(node_argv[i], "--tg_time"))
	{
	  tg_duration = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i], "--enable_oob"))
	{
	  enable_oob = true;
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
  if (config.profileMap.find("publisherB") != config.profileMap.end())
    {
      publisherB_send_mw.init(node_argc,
					     node_argv,
					     config.profileMap["publisherB"],
					     config.uuidMap["publisherB"],
					     enable_oob);
      if ( tg_duration < 0 )
	publisherB_send_mw.set_duration(publisherB_send_mw.profile.period);
      else
	publisherB_send_mw.set_duration(tg_duration);
      fName = config.nodeName + "." + config.compName + ".publisherB.network.csv";
      publisherB_send_mw.set_output_filename(fName);
    }
  if (config.profileMap.find("publisherC") != config.profileMap.end())
    {
      publisherC_send_mw.init(node_argc,
					     node_argv,
					     config.profileMap["publisherC"],
					     config.uuidMap["publisherC"],
					     enable_oob);
      if ( tg_duration < 0 )
	publisherC_send_mw.set_duration(publisherC_send_mw.profile.period);
      else
	publisherC_send_mw.set_duration(tg_duration);
      fName = config.nodeName + "." + config.compName + ".publisherC.network.csv";
      publisherC_send_mw.set_output_filename(fName);
    }
  if (config.profileMap.find("publisherD") != config.profileMap.end())
    {
      publisherD_send_mw.init(node_argc,
					     node_argv,
					     config.profileMap["publisherD"],
					     config.uuidMap["publisherD"],
					     enable_oob);
      if ( tg_duration < 0 )
	publisherD_send_mw.set_duration(publisherD_send_mw.profile.period);
      else
	publisherD_send_mw.set_duration(tg_duration);
      fName = config.nodeName + "." + config.compName + ".publisherD.network.csv";
      publisherD_send_mw.set_output_filename(fName);
    }
  if (config.profileMap.find("publisherE") != config.profileMap.end())
    {
      publisherE_send_mw.init(node_argc,
					     node_argv,
					     config.profileMap["publisherE"],
					     config.uuidMap["publisherE"],
					     enable_oob);
      if ( tg_duration < 0 )
	publisherE_send_mw.set_duration(publisherE_send_mw.profile.period);
      else
	publisherE_send_mw.set_duration(tg_duration);
      fName = config.nodeName + "." + config.compName + ".publisherE.network.csv";
      publisherE_send_mw.set_output_filename(fName);
    }
#else
  max_data_length = 160;
#endif
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting senderBCDE::init_timer_operation");
#endif  
}
//# End Init Marker

void senderBCDE::publisherB_timer_operation(const NAMESPACE::TimerEvent& event)
{
  multi_node_sample::messageB msg;
  msg.uuid = publisherB_send_mw.get_uuid();
  msg.bytes.resize(max_data_length,0);
  double timerDelay = 0;
  try
    {
      timerDelay =
	publisherB_send_mw.send<multi_node_sample::messageB>(publisherB, msg);
    }
  catch ( Network::Exceeded_Production_Profile& ex )
    {
      logger->log("DEBUG","Prevented from sending on the network!");
    }

  if ( ros::Time::now() >= publisherB_send_mw.get_end_time() )
    {
      logger->log("DEBUG","writing output\n");
      publisherB_send_mw.record();
    }
  else
    {
      if (tg_misbehave)
	timerDelay -= 0.1;
#ifdef USE_ROSMOD    
      rosmod::ROSMOD_Callback_Options callback_options;
      callback_options.alias = "init_timer_operation";
      callback_options.priority = 99;
      callback_options.deadline.sec = 1;
      callback_options.deadline.nsec = 0;
#endif
      NAMESPACE::TimerOptions timer_options;
      timer_options = 
	NAMESPACE::TimerOptions
	(ros::Duration(timerDelay),
	 boost::bind(&senderBCDE::publisherB_timer_operation, this, _1),
	 &this->comp_queue,
#ifdef USE_ROSMOD     
	 callback_options,
#endif 
	 true);
      NAMESPACE::NodeHandle nh;
      publisherB_timer = nh.createTimer(timer_options);
    }
}
void senderBCDE::publisherC_timer_operation(const NAMESPACE::TimerEvent& event)
{
  multi_node_sample::messageC msg;
  msg.uuid = publisherC_send_mw.get_uuid();
  msg.bytes.resize(max_data_length,0);
  double timerDelay = 0;
  try
    {
      timerDelay =
	publisherC_send_mw.send<multi_node_sample::messageC>(publisherC, msg);
    }
  catch ( Network::Exceeded_Production_Profile& ex )
    {
      logger->log("DEBUG","Prevented from sending on the network!");
    }

  if ( ros::Time::now() >= publisherC_send_mw.get_end_time() )
    {
      logger->log("DEBUG","writing output\n");
      publisherC_send_mw.record();
    }
  else
    {
      if (tg_misbehave)
	timerDelay -= 0.1;
#ifdef USE_ROSMOD    
      rosmod::ROSMOD_Callback_Options callback_options;
      callback_options.alias = "init_timer_operation";
      callback_options.priority = 99;
      callback_options.deadline.sec = 1;
      callback_options.deadline.nsec = 0;
#endif
      NAMESPACE::TimerOptions timer_options;
      timer_options = 
	NAMESPACE::TimerOptions
	(ros::Duration(timerDelay),
	 boost::bind(&senderBCDE::publisherC_timer_operation, this, _1),
	 &this->comp_queue,
#ifdef USE_ROSMOD     
	 callback_options,
#endif 
	 true);
      NAMESPACE::NodeHandle nh;
      publisherC_timer = nh.createTimer(timer_options);
    }
}
void senderBCDE::publisherD_timer_operation(const NAMESPACE::TimerEvent& event)
{
  multi_node_sample::messageD msg;
  msg.uuid = publisherD_send_mw.get_uuid();
  msg.bytes.resize(max_data_length,0);
  double timerDelay = 0;
  try
    {
      timerDelay =
	publisherD_send_mw.send<multi_node_sample::messageD>(publisherD, msg);
    }
  catch ( Network::Exceeded_Production_Profile& ex )
    {
      logger->log("DEBUG","Prevented from sending on the network!");
    }

  if ( ros::Time::now() >= publisherD_send_mw.get_end_time() )
    {
      logger->log("DEBUG","writing output\n");
      publisherD_send_mw.record();
    }
  else
    {
      if (tg_misbehave)
	timerDelay -= 0.1;
#ifdef USE_ROSMOD    
      rosmod::ROSMOD_Callback_Options callback_options;
      callback_options.alias = "init_timer_operation";
      callback_options.priority = 99;
      callback_options.deadline.sec = 1;
      callback_options.deadline.nsec = 0;
#endif
      NAMESPACE::TimerOptions timer_options;
      timer_options = 
	NAMESPACE::TimerOptions
	(ros::Duration(timerDelay),
	 boost::bind(&senderBCDE::publisherD_timer_operation, this, _1),
	 &this->comp_queue,
#ifdef USE_ROSMOD     
	 callback_options,
#endif 
	 true);
      NAMESPACE::NodeHandle nh;
      publisherD_timer = nh.createTimer(timer_options);
    }
}
void senderBCDE::publisherE_timer_operation(const NAMESPACE::TimerEvent& event)
{
  multi_node_sample::messageE msg;
  msg.uuid = publisherE_send_mw.get_uuid();
  msg.bytes.resize(max_data_length,0);
  double timerDelay = 0;
  try
    {
      timerDelay =
	publisherE_send_mw.send<multi_node_sample::messageE>(publisherE, msg);
    }
  catch ( Network::Exceeded_Production_Profile& ex )
    {
      logger->log("DEBUG","Prevented from sending on the network!");
    }

  if ( ros::Time::now() >= publisherE_send_mw.get_end_time() )
    {
      logger->log("DEBUG","writing output\n");
      publisherE_send_mw.record();
    }
  else
    {
      if (tg_misbehave)
	timerDelay -= 0.1;
#ifdef USE_ROSMOD    
      rosmod::ROSMOD_Callback_Options callback_options;
      callback_options.alias = "init_timer_operation";
      callback_options.priority = 99;
      callback_options.deadline.sec = 1;
      callback_options.deadline.nsec = 0;
#endif
      NAMESPACE::TimerOptions timer_options;
      timer_options = 
	NAMESPACE::TimerOptions
	(ros::Duration(timerDelay),
	 boost::bind(&senderBCDE::publisherE_timer_operation, this, _1),
	 &this->comp_queue,
#ifdef USE_ROSMOD     
	 callback_options,
#endif 
	 true);
      NAMESPACE::NodeHandle nh;
      publisherE_timer = nh.createTimer(timer_options);
    }
}

// Timer Callback - timerB
//# Start timerB_operation Marker
void senderBCDE::timerB_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering senderBCDE::timerB_operation");
#endif
  // Business Logic for timerB_operation
#if 0
  ros::Time now = ros::Time::now();
  timespec current_time;
  current_time.tv_sec = now.sec;
  current_time.tv_nsec = now.nsec;
  double offset = publisherB_send_mw.profile.getOffset(current_time);
  double period = publisherB_send_mw.profile.period;
  uint64_t message_len = max_data_length + sin(offset * 2 * M_PI / period) * max_data_length * multiplier;

  multi_node_sample::messageB msg;
  msg.uuid = publisherB_send_mw.get_uuid();
  msg.bytes.resize(message_len,0);
  double timerDelay = 0;
  try
    {
      timerDelay =
	publisherB_send_mw.send<multi_node_sample::messageB>(publisherB, msg);
    }
  catch ( Network::Exceeded_Production_Profile& ex )
    {
      logger->log("DEBUG","Prevented from sending on the network!");
    }

  if ( ros::Time::now() >= publisherB_send_mw.get_end_time() )
    {
      logger->log("DEBUG","writing output\n");
      publisherB_send_mw.record();
      timerB.stop();
    }
#else

  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Waiting Callback Priority: %d",
				comp_queue.getWaitingCallbackPriority());
  
  double now = ros::Time::now().toSec();
  double period = _profile_period;
  double offset = fmod(now, period);
  uint64_t message_len = max_data_length + sin(offset * 2 * M_PI / period) * max_data_length * multiplier;

  multi_node_sample::messageB msg;
  msg.uuid = 0;
  msg.bytes.resize(message_len,0);
  publisherB.publish(msg);
#endif
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting senderBCDE::timerB_operation");
#endif
}
//# End timerB_operation Marker
// Timer Callback - timerC
//# Start timerC_operation Marker
void senderBCDE::timerC_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering senderBCDE::timerC_operation");
#endif
  // Business Logic for timerC_operation
#if 0
  ros::Time now = ros::Time::now();
  timespec current_time;
  current_time.tv_sec = now.sec;
  current_time.tv_nsec = now.nsec;
  double offset = publisherC_send_mw.profile.getOffset(current_time);
  double period = publisherC_send_mw.profile.period;
  uint64_t message_len = max_data_length + sin(offset * 2 * M_PI / period) * max_data_length * multiplier;

  multi_node_sample::messageC msg;
  msg.uuid = publisherC_send_mw.get_uuid();
  msg.bytes.resize(message_len,0);
  double timerDelay = 0;
  try
    {
      timerDelay =
	publisherC_send_mw.send<multi_node_sample::messageC>(publisherC, msg);
    }
  catch ( Network::Exceeded_Production_Profile& ex )
    {
      logger->log("DEBUG","Prevented from sending on the network!");
    }

  if ( ros::Time::now() >= publisherC_send_mw.get_end_time() )
    {
      logger->log("DEBUG","writing output\n");
      publisherC_send_mw.record();
      timerC.stop();
    }
#else
  double now = ros::Time::now().toSec();
  double period = _profile_period;
  double offset = fmod(now, period);
  uint64_t message_len = max_data_length + sin(offset * 2 * M_PI / period) * max_data_length * multiplier;

  multi_node_sample::messageC msg;
  msg.uuid = 0;
  msg.bytes.resize(message_len,0);
  publisherC.publish(msg);
#endif
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting senderBCDE::timerC_operation");
#endif
}
//# End timerC_operation Marker
// Timer Callback - timerD
//# Start timerD_operation Marker
void senderBCDE::timerD_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering senderBCDE::timerD_operation");
#endif
  // Business Logic for timerD_operation
#if 0
  ros::Time now = ros::Time::now();
  timespec current_time;
  current_time.tv_sec = now.sec;
  current_time.tv_nsec = now.nsec;
  double offset = publisherD_send_mw.profile.getOffset(current_time);
  double period = publisherD_send_mw.profile.period;
  uint64_t message_len = max_data_length + sin(offset * 2 * M_PI / period) * max_data_length * multiplier;

  multi_node_sample::messageD msg;
  msg.uuid = publisherD_send_mw.get_uuid();
  msg.bytes.resize(message_len,0);
  double timerDelay = 0;
  try
    {
      timerDelay =
	publisherD_send_mw.send<multi_node_sample::messageD>(publisherD, msg);
    }
  catch ( Network::Exceeded_Production_Profile& ex )
    {
      logger->log("DEBUG","Prevented from sending on the network!");
    }

  if ( ros::Time::now() >= publisherD_send_mw.get_end_time() )
    {
      logger->log("DEBUG","writing output\n");
      publisherD_send_mw.record();
      timerD.stop();
    }
#else
  double now = ros::Time::now().toSec();
  double period = _profile_period;
  double offset = fmod(now, period);
  uint64_t message_len = max_data_length + sin(offset * 2 * M_PI / period) * max_data_length * multiplier;

  multi_node_sample::messageD msg;
  msg.uuid = 0;
  msg.bytes.resize(message_len,0);
  publisherD.publish(msg);
#endif
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting senderBCDE::timerD_operation");
#endif
}
//# End timerD_operation Marker
// Timer Callback - timerE
//# Start timerE_operation Marker
void senderBCDE::timerE_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering senderBCDE::timerE_operation");
#endif
  // Business Logic for timerE_operation
#if 0
  ros::Time now = ros::Time::now();
  timespec current_time;
  current_time.tv_sec = now.sec;
  current_time.tv_nsec = now.nsec;
  double offset = publisherE_send_mw.profile.getOffset(current_time);
  double period = publisherE_send_mw.profile.period;
  uint64_t message_len = max_data_length + sin(offset * 2 * M_PI / period) * max_data_length * multiplier;

  multi_node_sample::messageE msg;
  msg.uuid = publisherE_send_mw.get_uuid();
  msg.bytes.resize(message_len,0);
  double timerDelay = 0;
  try
    {
      timerDelay =
	publisherE_send_mw.send<multi_node_sample::messageE>(publisherE, msg);
    }
  catch ( Network::Exceeded_Production_Profile& ex )
    {
      logger->log("DEBUG","Prevented from sending on the network!");
    }

  if ( ros::Time::now() >= publisherE_send_mw.get_end_time() )
    {
      logger->log("DEBUG","writing output\n");
      publisherE_send_mw.record();
      timerE.stop();
    }
#else
  double now = ros::Time::now().toSec();
  double period = _profile_period;
  double offset = fmod(now, period);
  uint64_t message_len = max_data_length + sin(offset * 2 * M_PI / period) * max_data_length * multiplier;

  multi_node_sample::messageE msg;
  msg.uuid = 0;
  msg.bytes.resize(message_len,0);
  publisherE.publish(msg);
#endif
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting senderBCDE::timerE_operation");
#endif
}
//# End timerE_operation Marker


// Destructor - Cleanup Ports & Timers
senderBCDE::~senderBCDE()
{
  timerB.stop();
  timerC.stop();
  timerD.stop();
  timerE.stop();
  publisherB.shutdown();
  publisherC.shutdown();
  publisherD.shutdown();
  publisherE.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void senderBCDE::startUp()
{
  NAMESPACE::NodeHandle nh;
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
  std::string log_file_path = pwd + config.nodeName + "." + config.compName + ".log"; 

  logger->create_file("/var/log/" + config.nodeName + "." + config.compName + ".log");
  logger->set_is_periodic(config.is_periodic_logging);
  logger->set_max_log_unit(config.periodic_log_unit);

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->create_file("/var/log/ROSMOD_DEBUG." + config.nodeName + "." + config.compName + ".log");
  comp_queue.ROSMOD_LOGGER->set_is_periodic(config.is_periodic_logging);
  comp_queue.ROSMOD_LOGGER->set_max_log_unit(config.periodic_log_unit);
#endif    
  
#ifdef USE_ROSMOD 
  this->comp_queue.scheduling_scheme = config.schedulingScheme;
  rosmod::ROSMOD_Callback_Options callback_options;
#endif  
  // Configure all publishers associated with this component
  // Component Publisher - publisherB
  advertiseName = "messageB";
  if (config.portGroupMap.find("publisherB") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["publisherB"];
  this->publisherB = nh.advertise<multi_node_sample::messageB>(advertiseName.c_str(), 1000);
  // Component Publisher - publisherC
  advertiseName = "messageC";
  if (config.portGroupMap.find("publisherC") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["publisherC"];
  this->publisherC = nh.advertise<multi_node_sample::messageC>(advertiseName.c_str(), 1000);
  // Component Publisher - publisherD
  advertiseName = "messageD";
  if (config.portGroupMap.find("publisherD") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["publisherD"];
  this->publisherD = nh.advertise<multi_node_sample::messageD>(advertiseName.c_str(), 1000);
  // Component Publisher - publisherE
  advertiseName = "messageE";
  if (config.portGroupMap.find("publisherE") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["publisherE"];
  this->publisherE = nh.advertise<multi_node_sample::messageE>(advertiseName.c_str(), 1000);


  if (config.num_comps_to_sync > 1 )
    {
      // Synchronize components now that all publishers and servers have been initialized
      this->comp_sync_pub = nh.advertise<std_msgs::Bool>("component_synchronization", 1000);
  
#ifdef USE_ROSMOD  
      rosmod::SubscribeOptions comp_sync_sub_options;
      rosmod::ROSMOD_Callback_Options sync_callback_options;
#else
      ros::SubscribeOptions comp_sync_sub_options;
#endif
      ros::Duration(config.comp_sync_timeout/2.0).sleep();
      comp_sync_sub_options = NAMESPACE::SubscribeOptions::create<std_msgs::Bool>
	("component_synchronization",
	 1000,
	 boost::bind(&senderBCDE::component_sync_operation, this, _1),
	 NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD     
	 &this->comp_queue,
	 sync_callback_options);
#else
         &this->comp_queue);
#endif
      this->comp_sync_sub = nh.subscribe(comp_sync_sub_options);

      ros::Time now = ros::Time::now();
      while ( this->comp_sync_sub.getNumPublishers() < this->config.num_comps_to_sync &&
	      (ros::Time::now() - now) < ros::Duration(config.comp_sync_timeout))
	ros::Duration(0.1).sleep();
      ros::Duration(config.comp_sync_timeout/2.0).sleep();
      this->comp_sync_sub.shutdown();  
      this->comp_sync_pub.shutdown();
    }


  // Init Timer
#ifdef USE_ROSMOD    
  callback_options.alias = "init_timer_operation";
  callback_options.priority = 99;
  callback_options.deadline.sec = 1;
  callback_options.deadline.nsec = 0;
#endif
  NAMESPACE::TimerOptions timer_options;
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(-1),
     boost::bind(&senderBCDE::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();
#ifdef USE_ROSMOD   
  callback_options.alias = "timerB_operation";
  callback_options.priority = 90;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 1000000;
#endif
  // Component Timer - timerB
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(5.0),
     boost::bind(&senderBCDE::timerB_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->timerB = nh.createTimer(timer_options);
#ifdef USE_ROSMOD   
  callback_options.alias = "timerC_operation";
  callback_options.priority = 80;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 1000000;
#endif
  // Component Timer - timerC
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(1.0),
     boost::bind(&senderBCDE::timerC_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->timerC = nh.createTimer(timer_options);
#ifdef USE_ROSMOD   
  callback_options.alias = "timerD_operation";
  callback_options.priority = 70;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 1000000;
#endif
  // Component Timer - timerD
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(1.5),
     boost::bind(&senderBCDE::timerD_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->timerD = nh.createTimer(timer_options);
#ifdef USE_ROSMOD   
  callback_options.alias = "timerE_operation";
  callback_options.priority = 60;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 1000000;
#endif
  // Component Timer - timerE
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(2.0),
     boost::bind(&senderBCDE::timerE_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->timerE = nh.createTimer(timer_options);


  this->init_timer.start();
  this->timerB.start();
  this->timerC.start();
  this->timerD.start();
  this->timerE.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new senderBCDE(config,argc,argv);
  }
}
