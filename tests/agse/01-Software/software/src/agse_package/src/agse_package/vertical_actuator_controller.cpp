#include "agse_package/vertical_actuator_controller.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void vertical_actuator_controller::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering vertical_actuator_controller::init_timer_operation");
#endif
  // Initialize Here
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting vertical_actuator_controller::init_timer_operation");
#endif  
}
//# End Init Marker



// Subscriber Operation - controlInputs_sub
//# Start controlInputs_sub_operation Marker
void vertical_actuator_controller::controlInputs_sub_operation(const agse_package::controlInputs::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering vertical_actuator_controller::controlInputs_sub_operation");
#endif
  // Business Logic for controlInputs_sub_operation

  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting vertical_actuator_controller::controlInputs_sub_operation");
#endif
}
//# End controlInputs_sub_operation Marker

// Server Operation - verticalPos_server
//# Start verticalPos_operation Marker
bool vertical_actuator_controller::verticalPos_operation(agse_package::verticalPos::Request  &req,
  agse_package::verticalPos::Response &res)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering vertical_actuator_controller::verticalPos_operation");
#endif
  // Business Logic for verticalPos_server_operation

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting vertical_actuator_controller::verticalPos_operation");
#endif
  return true;
}
//# End verticalPos_operation Marker

// Timer Callback - verticalPosTimer
//# Start verticalPosTimer_operation Marker
void vertical_actuator_controller::verticalPosTimer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering vertical_actuator_controller::verticalPosTimer_operation");
#endif
  // Business Logic for verticalPosTimer_operation

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting vertical_actuator_controller::verticalPosTimer_operation");
#endif
}
//# End verticalPosTimer_operation Marker


// Destructor - Cleanup Ports & Timers
vertical_actuator_controller::~vertical_actuator_controller()
{
  verticalPosTimer.stop();
  controlInputs_sub.shutdown();
  verticalPos_server.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void vertical_actuator_controller::startUp()
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
  // Configure all provided services associated with this component
#ifdef USE_ROSMOD  
  callback_options.alias = "verticalPos_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec =0;
  callback_options.deadline.nsec = 100000000;
#endif    
  // Component Server - verticalPos_server
  advertiseName = "verticalPos";
  if (config.portGroupMap.find("verticalPos_server") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["verticalPos_server"];
  NAMESPACE::AdvertiseServiceOptions verticalPos_server_server_options;
  verticalPos_server_server_options = NAMESPACE::AdvertiseServiceOptions::create<agse_package::verticalPos>
      (advertiseName.c_str(),
       boost::bind(&vertical_actuator_controller::verticalPos_operation, this, _1, _2),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD       
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif
  this->verticalPos_server = nh.advertiseService(verticalPos_server_server_options);

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
	 boost::bind(&vertical_actuator_controller::component_sync_operation, this, _1),
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

  // Configure all subscribers associated with this component
#ifdef USE_ROSMOD 
  callback_options.alias = "controlInputs_sub_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif  
  // Component Subscriber - controlInputs_sub
  advertiseName = "controlInputs";
  if (config.portGroupMap.find("controlInputs_sub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["controlInputs_sub"];
  NAMESPACE::SubscribeOptions controlInputs_sub_options;
  controlInputs_sub_options = NAMESPACE::SubscribeOptions::create<agse_package::controlInputs>
      (advertiseName.c_str(),
       1000,
       boost::bind(&vertical_actuator_controller::controlInputs_sub_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  controlInputs_sub_options.transport_hints = NAMESPACE::TransportHints().udp();
  this->controlInputs_sub = nh.subscribe(controlInputs_sub_options);

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
     boost::bind(&vertical_actuator_controller::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();
#ifdef USE_ROSMOD   
  callback_options.alias = "verticalPosTimer_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif
  // Component Timer - verticalPosTimer
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(0.01),
     boost::bind(&vertical_actuator_controller::verticalPosTimer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->verticalPosTimer = nh.createTimer(timer_options);


  this->init_timer.start();
  this->verticalPosTimer.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new vertical_actuator_controller(config,argc,argv);
  }
}
