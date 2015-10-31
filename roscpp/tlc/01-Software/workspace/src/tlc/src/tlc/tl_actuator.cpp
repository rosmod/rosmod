#include "tlc/tl_actuator.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void tl_actuator::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering tl_actuator::init_timer_operation");
#endif
  // Initialize Here
  _id = "V1";
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
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting tl_actuator::init_timer_operation");
#endif  
}
//# End Init Marker



// Subscriber Operation - ryg_control_sub
//# Start ryg_control_sub_operation Marker
void tl_actuator::ryg_control_sub_operation(const tlc::ryg_control::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering tl_actuator::ryg_control_sub_operation");
#endif
  // Business Logic for ryg_control_sub_operation
  tlc::tlc_set_ryg_state state;
  state.request.intersection_name = received_data->intersection_name;
  if ( !_id.compare(state.request.intersection_name) )
    {
      state.request.ryg_state = received_data->state;
      logger->log("DEBUG", "Setting TL state for :: %s , %s",
		  received_data->intersection_name.c_str(),
		  received_data->state.c_str());
      tlc_set_ryg_state_client.call(state);
    }
  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting tl_actuator::ryg_control_sub_operation");
#endif
}
//# End ryg_control_sub_operation Marker


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
     boost::bind(&tl_actuator::component_sync_operation, this, _1),
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


#ifdef USE_ROSMOD 
  callback_options.alias = "ryg_control_sub_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif  
  // Component Subscriber - ryg_control_sub
  advertiseName = "ryg_control";
  if (config.portGroupMap.find("ryg_control_sub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["ryg_control_sub"];
  NAMESPACE::SubscribeOptions ryg_control_sub_options;
  ryg_control_sub_options = NAMESPACE::SubscribeOptions::create<tlc::ryg_control>
      (advertiseName.c_str(),
       1000,
       boost::bind(&tl_actuator::ryg_control_sub_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  this->ryg_control_sub = nh.subscribe(ryg_control_sub_options);

  // Configure all required services associated with this component
  // Component Client - tlc_set_ryg_state_client
  advertiseName = "tlc_set_ryg_state";
  if (config.portGroupMap.find("tlc_set_ryg_state_client") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["tlc_set_ryg_state_client"];
  this->tlc_set_ryg_state_client = nh.serviceClient<tlc::tlc_set_ryg_state>(advertiseName.c_str(), true); 

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
     boost::bind(&tl_actuator::init_timer_operation, this, _1),
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
    return new tl_actuator(config,argc,argv);
  }
}
