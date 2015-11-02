#include "tlc/tl_sensor.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void tl_sensor::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering tl_sensor::init_timer_operation");
#endif
  // Initialize Here
  _last_state = "";
  _id = "";
  for (int i=0;i<node_argc;i++)
    {
      if (!strcmp(node_argv[i],"--id"))
	{
	  _id = node_argv[i+1];
	}
    }
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting tl_sensor::init_timer_operation");
#endif  
}
//# End Init Marker


// Timer Callback - tl_update_timer
//# Start tl_update_timer_operation Marker
void tl_sensor::tl_update_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering tl_sensor::tl_update_timer_operation");
#endif
  // Business Logic for tl_update_timer_operation
  logger->log("DEBUG","Getting TL sensor state for :: %s", _id.c_str());
  tlc::tlc_get_ryg_state sumo_ryg_state;
  sumo_ryg_state.request.intersection_name = _id;
  if ( tlc_get_ryg_state_client.call(sumo_ryg_state))
    {
      _last_state = sumo_ryg_state.response.ryg_state;
    }
  else
    {
      logger->log("ERROR","client not found for TLC_GET_RYG_STATE");
    }
  tlc::ryg_state local_ryg_state;
  local_ryg_state.intersection_name = _id;
  local_ryg_state.state = _last_state;
  logger->log("DEBUG","Publishing TL sensor state for :: %s : %s", _id.c_str(), _last_state.c_str());
  ryg_state_pub.publish(local_ryg_state);

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting tl_sensor::tl_update_timer_operation");
#endif
}
//# End tl_update_timer_operation Marker


// Destructor - Cleanup Ports & Timers
tl_sensor::~tl_sensor()
{
  tl_update_timer.stop();
  ryg_state_pub.shutdown();
  tlc_get_ryg_state_client.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void tl_sensor::startUp()
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
  // Component Publisher - ryg_state_pub
  advertiseName = "ryg_state";
  if (config.portGroupMap.find("ryg_state_pub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["ryg_state_pub"];
  this->ryg_state_pub = nh.advertise<tlc::ryg_state>(advertiseName.c_str(), 1000);


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
     boost::bind(&tl_sensor::component_sync_operation, this, _1),
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


  // Configure all required services associated with this component
  // Component Client - tlc_get_ryg_state_client
  advertiseName = "tlc_get_ryg_state";
  if (config.portGroupMap.find("tlc_get_ryg_state_client") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["tlc_get_ryg_state_client"];
  this->tlc_get_ryg_state_client = nh.serviceClient<tlc::tlc_get_ryg_state>(advertiseName.c_str(), true); 

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
     boost::bind(&tl_sensor::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();
#ifdef USE_ROSMOD   
  callback_options.alias = "tl_update_timer_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif
  // Component Timer - tl_update_timer
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(0.1),
     boost::bind(&tl_sensor::tl_update_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->tl_update_timer = nh.createTimer(timer_options);


  this->init_timer.start();
  this->tl_update_timer.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new tl_sensor(config,argc,argv);
  }
}
