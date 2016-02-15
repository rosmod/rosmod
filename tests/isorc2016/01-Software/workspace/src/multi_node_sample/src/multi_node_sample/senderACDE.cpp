#include "multi_node_sample/senderACDE.hpp"

//# Start User Globals Marker
double multiplier = 0.3;
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void senderACDE::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering senderACDE::init_timer_operation");
#endif
  // Initialize Here
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

  if (config.profileMap.find("publisherA") != config.profileMap.end())
    {
      publisherA_send_mw.init(node_argc,
					     node_argv,
					     config.profileMap["publisherA"],
					     config.uuidMap["publisherA"],
					     enable_oob);
      if ( tg_duration < 0 )
	publisherA_send_mw.set_duration(publisherA_send_mw.profile.period);
      else
	publisherA_send_mw.set_duration(tg_duration);
      fName = config.nodeName + "." + config.compName + ".publisherA.network.csv";
      publisherA_send_mw.set_output_filename(fName);
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
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting senderACDE::init_timer_operation");
#endif  
}
//# End Init Marker

// Timer Callback - timerA
//# Start timerA_operation Marker
void senderACDE::timerA_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering senderACDE::timerA_operation");
#endif
  // Business Logic for timerA_operation
  ros::Time now = ros::Time::now();
  timespec current_time;
  current_time.tv_sec = now.sec;
  current_time.tv_nsec = now.nsec;
  double offset = publisherA_send_mw.profile.getOffset(current_time);
  double period = publisherA_send_mw.profile.period;
  uint64_t message_len = max_data_length + sin(offset * 2 * M_PI / period) * max_data_length * multiplier;

  multi_node_sample::messageA msg;
  msg.uuid = publisherA_send_mw.get_uuid();
  msg.bytes.resize(message_len,0);
  double timerDelay = 0;
  try
    {
      timerDelay =
	publisherA_send_mw.send<multi_node_sample::messageA>(publisherA, msg);
    }
  catch ( Network::Exceeded_Production_Profile& ex )
    {
      logger->log("DEBUG","Prevented from sending on the network!");
    }

  if ( ros::Time::now() >= publisherA_send_mw.get_end_time() )
    {
      logger->log("DEBUG","writing output\n");
      publisherA_send_mw.record();
      timerA.stop();
    }

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting senderACDE::timerA_operation");
#endif
}
//# End timerA_operation Marker
// Timer Callback - timerC
//# Start timerC_operation Marker
void senderACDE::timerC_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering senderACDE::timerC_operation");
#endif
  // Business Logic for timerC_operation
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

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting senderACDE::timerC_operation");
#endif
}
//# End timerC_operation Marker
// Timer Callback - timerD
//# Start timerD_operation Marker
void senderACDE::timerD_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering senderACDE::timerD_operation");
#endif
  // Business Logic for timerD_operation
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

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting senderACDE::timerD_operation");
#endif
}
//# End timerD_operation Marker
// Timer Callback - timerE
//# Start timerE_operation Marker
void senderACDE::timerE_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering senderACDE::timerE_operation");
#endif
  // Business Logic for timerE_operation
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

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting senderACDE::timerE_operation");
#endif
}
//# End timerE_operation Marker


// Destructor - Cleanup Ports & Timers
senderACDE::~senderACDE()
{
  timerA.stop();
  timerC.stop();
  timerD.stop();
  timerE.stop();
  publisherA.shutdown();
  publisherC.shutdown();
  publisherD.shutdown();
  publisherE.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void senderACDE::startUp()
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

  logger->create_file(pwd + config.nodeName + "." + config.compName + ".log");
  logger->set_is_periodic(config.is_periodic_logging);
  logger->set_max_log_unit(config.periodic_log_unit);

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->create_file(pwd + "ROSMOD_DEBUG." + config.nodeName + "." + config.compName + ".log");
  comp_queue.ROSMOD_LOGGER->set_is_periodic(config.is_periodic_logging);
  comp_queue.ROSMOD_LOGGER->set_max_log_unit(config.periodic_log_unit);
#endif    
  
#ifdef USE_ROSMOD 
  this->comp_queue.scheduling_scheme = config.schedulingScheme;
  rosmod::ROSMOD_Callback_Options callback_options;
#endif  
  // Configure all publishers associated with this component
  // Component Publisher - publisherA
  advertiseName = "messageA";
  if (config.portGroupMap.find("publisherA") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["publisherA"];
  this->publisherA = nh.advertise<multi_node_sample::messageA>(advertiseName.c_str(), 1000);
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


  // Synchronize components now that all publishers and servers have been initialized
  this->comp_sync_pub = nh.advertise<std_msgs::Bool>("component_synchronization", 1000);
  
  ros::Duration(0.5).sleep();

#ifdef USE_ROSMOD  
  rosmod::SubscribeOptions comp_sync_sub_options;
  rosmod::ROSMOD_Callback_Options sync_callback_options;
#else
  ros::SubscribeOptions comp_sync_sub_options;
#endif
  
  comp_sync_sub_options = NAMESPACE::SubscribeOptions::create<std_msgs::Bool>
    ("component_synchronization",
     1000,
     boost::bind(&senderACDE::component_sync_operation, this, _1),
     NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD     
     &this->comp_queue,
     sync_callback_options);
#else
     &this->comp_queue);
#endif
  this->comp_sync_sub = nh.subscribe(comp_sync_sub_options);

  ros::Time now = ros::Time::now();
  int num_publishers = this->comp_sync_sub.getNumPublishers();
  while ( num_publishers < this->config.num_comps_to_sync &&
	  (ros::Time::now() - now) < ros::Duration(config.comp_sync_timeout))
    {
      num_publishers = this->comp_sync_sub.getNumPublishers();
      std::cout << "Num publishers: " << num_publishers << std::endl;
      ros::Duration(0.1).sleep();
    }
  ros::Duration(0.5).sleep();
  this->comp_sync_sub.shutdown();  
  this->comp_sync_pub.shutdown();


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
     boost::bind(&senderACDE::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();
#ifdef USE_ROSMOD   
  callback_options.alias = "timerA_operation";
  callback_options.priority = 90;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 50000000;
#endif
  // Component Timer - timerA
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(0.05),
     boost::bind(&senderACDE::timerA_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->timerA = nh.createTimer(timer_options);
#ifdef USE_ROSMOD   
  callback_options.alias = "timerC_operation";
  callback_options.priority = 80;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 60000000;
#endif
  // Component Timer - timerC
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(0.06),
     boost::bind(&senderACDE::timerC_operation, this, _1),
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
  callback_options.deadline.nsec = 70000000;
#endif
  // Component Timer - timerD
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(0.07),
     boost::bind(&senderACDE::timerD_operation, this, _1),
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
  callback_options.deadline.nsec = 80000000;
#endif
  // Component Timer - timerE
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(0.08),
     boost::bind(&senderACDE::timerE_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->timerE = nh.createTimer(timer_options);


  this->init_timer.start();
  this->timerA.start();
  this->timerC.start();
  this->timerD.start();
  this->timerE.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new senderACDE(config,argc,argv);
  }
}
