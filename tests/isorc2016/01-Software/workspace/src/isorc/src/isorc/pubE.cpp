#include "isorc/pubE.hpp"

//# Start User Globals Marker
double multiplier = 0.3;
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void pubE::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering pubE::init_timer_operation");
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
  if (config.profileMap.find("e_pub") != config.profileMap.end())
    {
      e_pub_send_mw.init(node_argc,
					     node_argv,
					     config.profileMap["e_pub"],
					     config.uuidMap["e_pub"],
					     enable_oob);
      if ( tg_duration < 0 )
	e_pub_send_mw.set_duration(e_pub_send_mw.profile.period);
      else
	e_pub_send_mw.set_duration(tg_duration);
      fName = "/var/log/" + config.nodeName + "." + config.compName + ".e_pub.network.csv";
      e_pub_send_mw.set_output_filename(fName);
    }
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting pubE::init_timer_operation");
#endif  
}
//# End Init Marker

void pubE::e_pub_timer_operation(const NAMESPACE::TimerEvent& event)
{
  isorc::messageE msg;
  msg.uuid = e_pub_send_mw.get_uuid();
  msg.bytes.resize(max_data_length,0);
  double timerDelay = 0;
  try
    {
      timerDelay =
	e_pub_send_mw.send<isorc::messageE>(e_pub, msg);
    }
  catch ( Network::Exceeded_Production_Profile& ex )
    {
      logger->log("DEBUG","Prevented from sending on the network!");
    }

  if ( ros::Time::now() >= e_pub_send_mw.get_end_time() )
    {
      logger->log("DEBUG","writing output\n");
      e_pub_send_mw.record();
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
	 boost::bind(&pubE::e_pub_timer_operation, this, _1),
	 &this->comp_queue,
#ifdef USE_ROSMOD     
	 callback_options,
#endif 
	 true);
      NAMESPACE::NodeHandle nh;
      e_pub_timer = nh.createTimer(timer_options);
    }
}

// Timer Callback - pub_timer
//# Start pub_timer_operation Marker
void pubE::pub_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering pubE::pub_timer_operation");
#endif
  // Business Logic for pub_timer_operation
  ros::Time now = ros::Time::now();
  timespec current_time;
  current_time.tv_sec = now.sec;
  current_time.tv_nsec = now.nsec;
  double offset = e_pub_send_mw.profile.getOffset(current_time);
  double period = e_pub_send_mw.profile.period;
  uint64_t message_len = max_data_length + sin(offset * 2 * M_PI / period) * max_data_length * multiplier;

  isorc::messageE msg;
  msg.uuid = e_pub_send_mw.get_uuid();
  msg.bytes.resize(message_len,0);
  double timerDelay = 0;
  try
    {
      timerDelay =
	e_pub_send_mw.send<isorc::messageE>(e_pub, msg);
    }
  catch ( Network::Exceeded_Production_Profile& ex )
    {
      logger->log("DEBUG","Prevented from sending on the network!");
    }

  if ( ros::Time::now() >= e_pub_send_mw.get_end_time() )
    {
      logger->log("DEBUG","writing output\n");
      e_pub_send_mw.record();
      pub_timer.stop();
    }

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting pubE::pub_timer_operation");
#endif
}
//# End pub_timer_operation Marker


// Destructor - Cleanup Ports & Timers
pubE::~pubE()
{
  pub_timer.stop();
  e_pub.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void pubE::startUp()
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
  // Component Publisher - e_pub
  advertiseName = "messageE";
  if (config.portGroupMap.find("e_pub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["e_pub"];
  this->e_pub = nh.advertise<isorc::messageE>(advertiseName.c_str(), 1000);


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
	 boost::bind(&pubE::component_sync_operation, this, _1),
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
     boost::bind(&pubE::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();
#ifdef USE_ROSMOD   
  callback_options.alias = "pub_timer_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 50000000;
#endif
  // Component Timer - pub_timer
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(0.05),
     boost::bind(&pubE::pub_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->pub_timer = nh.createTimer(timer_options);


  this->init_timer.start();
  this->pub_timer.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new pubE(config,argc,argv);
  }
}
