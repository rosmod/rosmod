#include "tlc/tl_sensor_stream_ddos.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void tl_sensor_stream_ddos::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering tl_sensor_stream_ddos::init_timer_operation");
#endif
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
  NAMESPACE::NodeHandle nh;
  NAMESPACE::TimerOptions timer_options;
  if (config.profileMap.find("tl_sensor_stream_ddos_pub") != config.profileMap.end())
    {
      tl_sensor_stream_ddos_pub_send_mw.init(node_argc,
					     node_argv,
					     config.uuidMap["tl_sensor_stream_ddos_pub"],
					     config.profileMap["tl_sensor_stream_ddos_pub"]);
      if ( tg_duration < 0 )
	tl_sensor_stream_ddos_pub_send_mw.set_duration(tl_sensor_stream_ddos_pub_send_mw.profile.period);
      else
	tl_sensor_stream_ddos_pub_send_mw.set_duration(tg_duration);
      fName = config.nodeName + "." + config.compName + ".tl_sensor_stream_ddos_pub.network.csv";
      tl_sensor_stream_ddos_pub_send_mw.set_output_filename(fName);

#ifdef USE_ROSMOD    
      rosmod::ROSMOD_Callback_Options callback_options;
      callback_options.alias = "init_timer_operation";
      callback_options.priority = 99;
      callback_options.deadline.sec = 1;
      callback_options.deadline.nsec = 0;
#endif
      timer_options = 
	NAMESPACE::TimerOptions
	(ros::Duration(-1),
	 boost::bind(&tl_sensor_stream_ddos::tl_sensor_stream_ddos_pub_timer_operation, this, _1),
	 &this->comp_queue,
#ifdef USE_ROSMOD     
	 callback_options,
#endif 
	 true);
      tl_sensor_stream_ddos_pub_timer = nh.createTimer(timer_options);
    }
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting tl_sensor_stream_ddos::init_timer_operation");
#endif  
}
//# End Init Marker

void tl_sensor_stream_ddos::tl_sensor_stream_ddos_pub_timer_operation(const NAMESPACE::TimerEvent& event)
{
  tlc::ryg_state msg;
  msg.intersection_name = "random_name_here";
  msg.state = "bad_state";
  double timerDelay = 0;
  try
    {
      timerDelay =
	tl_sensor_stream_ddos_pub_send_mw.send<tlc::ryg_state>(tl_sensor_stream_ddos_pub, msg);
    }
  catch ( Network::Exceeded_Production_Profile& ex )
    {
      logger->log("DEBUG","Prevented from sending on the network!");
    }

  if ( ros::Time::now() >= tl_sensor_stream_ddos_pub_send_mw.get_end_time() )
    {
      logger->log("DEBUG","writing output\n");
      tl_sensor_stream_ddos_pub_send_mw.record();
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
	 boost::bind(&tl_sensor_stream_ddos::tl_sensor_stream_ddos_pub_timer_operation, this, _1),
	 &this->comp_queue,
#ifdef USE_ROSMOD     
	 callback_options,
#endif 
	 true);
      NAMESPACE::NodeHandle nh;
      tl_sensor_stream_ddos_pub_timer = nh.createTimer(timer_options);
    }
}


// Destructor - Cleanup Ports & Timers
tl_sensor_stream_ddos::~tl_sensor_stream_ddos()
{
  tl_sensor_stream_ddos_pub.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void tl_sensor_stream_ddos::startUp()
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
  // Component Publisher - tl_sensor_stream_ddos_pub
  advertiseName = "ryg_state";
  if (config.portGroupMap.find("tl_sensor_stream_ddos_pub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["tl_sensor_stream_ddos_pub"];
  this->tl_sensor_stream_ddos_pub = nh.advertise<tlc::ryg_state>(advertiseName.c_str(), 1000);


  // Synchronize components now that all publishers and servers have been initialized
  this->comp_sync_pub = nh.advertise<std_msgs::Bool>("component_synchronization", 1000);
  
#ifdef USE_ROSMOD  
  rosmod::SubscribeOptions comp_sync_sub_options;
  rosmod::ROSMOD_Callback_Options sync_callback_options;
#else
  ros::SubscribeOptions comp_sync_sub_options;
#endif
  
  comp_sync_sub_options = NAMESPACE::SubscribeOptions::create<std_msgs::Bool>
    ("component_synchronization",
     1000,
     boost::bind(&tl_sensor_stream_ddos::component_sync_operation, this, _1),
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
     boost::bind(&tl_sensor_stream_ddos::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();


  this->init_timer.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new tl_sensor_stream_ddos(config,argc,argv);
  }
}