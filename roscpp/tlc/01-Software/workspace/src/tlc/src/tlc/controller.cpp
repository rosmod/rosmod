#include "tlc/controller.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void controller::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering controller::init_timer_operation");
#endif
  // Initialize Here
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting controller::init_timer_operation");
#endif  
}
//# End Init Marker



// Subscriber Operation - ryg_state_sub
//# Start ryg_state_sub_operation Marker
void controller::ryg_state_sub_operation(const tlc::ryg_state::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering controller::ryg_state_sub_operation");
#endif
  // Business Logic for ryg_state_sub_operation

  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting controller::ryg_state_sub_operation");
#endif
}
//# End ryg_state_sub_operation Marker
// Subscriber Operation - e3_ingress
//# Start e3_ingress_operation Marker
void controller::e3_ingress_operation(const tlc::sensor_state::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering controller::e3_ingress_operation");
#endif
  // Business Logic for e3_ingress_operation

  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting controller::e3_ingress_operation");
#endif
}
//# End e3_ingress_operation Marker
// Subscriber Operation - e3_egress
//# Start e3_egress_operation Marker
void controller::e3_egress_operation(const tlc::sensor_state::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering controller::e3_egress_operation");
#endif
  // Business Logic for e3_egress_operation

  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting controller::e3_egress_operation");
#endif
}
//# End e3_egress_operation Marker

// Timer Callback - controller_timer
//# Start controller_timer_operation Marker
void controller::controller_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
	  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering controller::controller_timer_operation");
#endif
  // Business Logic for controller_timer_operation

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting controller::controller_timer_operation");
#endif
}
//# End controller_timer_operation Marker


// Destructor - Cleanup Ports & Timers
controller::~controller()
{
  controller_timer.stop();
  ryg_control_pub.shutdown();
  ryg_state_sub.shutdown();
  e3_ingress.shutdown();
  e3_egress.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void controller::startUp()
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

#ifdef USE_ROSMOD 
  callback_options.alias = "ryg_state_sub_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif  
  // Component Subscriber - ryg_state_sub
  advertiseName = "ryg_state";
  if (config.portGroupMap.find("ryg_state_sub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["ryg_state_sub"];
  NAMESPACE::SubscribeOptions ryg_state_sub_options;
  ryg_state_sub_options = NAMESPACE::SubscribeOptions::create<tlc::ryg_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller::ryg_state_sub_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  this->ryg_state_sub = nh.subscribe(ryg_state_sub_options);
#ifdef USE_ROSMOD 
  callback_options.alias = "e3_ingress_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif  
  // Component Subscriber - e3_ingress
  advertiseName = "sensor_state";
  if (config.portGroupMap.find("e3_ingress") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["e3_ingress"];
  NAMESPACE::SubscribeOptions e3_ingress_options;
  e3_ingress_options = NAMESPACE::SubscribeOptions::create<tlc::sensor_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller::e3_ingress_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  this->e3_ingress = nh.subscribe(e3_ingress_options);
#ifdef USE_ROSMOD 
  callback_options.alias = "e3_egress_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif  
  // Component Subscriber - e3_egress
  advertiseName = "sensor_state";
  if (config.portGroupMap.find("e3_egress") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["e3_egress"];
  NAMESPACE::SubscribeOptions e3_egress_options;
  e3_egress_options = NAMESPACE::SubscribeOptions::create<tlc::sensor_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller::e3_egress_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  this->e3_egress = nh.subscribe(e3_egress_options);

  // Component Publisher - ryg_control_pub
  advertiseName = "ryg_control";
  if (config.portGroupMap.find("ryg_control_pub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["ryg_control_pub"];
  this->ryg_control_pub = nh.advertise<tlc::ryg_control>(advertiseName.c_str(), 1000);

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
     boost::bind(&controller::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();
#ifdef USE_ROSMOD   
  callback_options.alias = "controller_timer_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif
  // Component Timer - controller_timer
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(0.1),
     boost::bind(&controller::controller_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->controller_timer = nh.createTimer(timer_options);



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
     boost::bind(&controller::component_sync_operation, this, _1),
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

  this->init_timer.start();
  this->controller_timer.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new controller(config,argc,argv);
  }
}
