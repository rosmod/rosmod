#include "agse_package/servo_controller.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void servo_controller::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering servo_controller::init_timer_operation");
#endif
  // Initialize Here
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting servo_controller::init_timer_operation");
#endif  
}
//# End Init Marker



// Subscriber Operation - controlInputs_sub
//# Start controlInputs_sub_operation Marker
void servo_controller::controlInputs_sub_operation(const agse_package::controlInputs::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering servo_controller::controlInputs_sub_operation");
#endif
  // Business Logic for controlInputs_sub_operation

  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting servo_controller::controlInputs_sub_operation");
#endif
}
//# End controlInputs_sub_operation Marker

// Server Operation - armRotation_server
//# Start armRotation_operation Marker
bool servo_controller::armRotation_operation(agse_package::armRotation::Request  &req,
  agse_package::armRotation::Response &res)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering servo_controller::armRotation_operation");
#endif
  // Business Logic for armRotation_server_operation

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting servo_controller::armRotation_operation");
#endif
  return true;
}
//# End armRotation_operation Marker
// Server Operation - gripperPos_server
//# Start gripperPos_operation Marker
bool servo_controller::gripperPos_operation(agse_package::gripperPos::Request  &req,
  agse_package::gripperPos::Response &res)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering servo_controller::gripperPos_operation");
#endif
  // Business Logic for gripperPos_server_operation

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting servo_controller::gripperPos_operation");
#endif
  return true;
}
//# End gripperPos_operation Marker
// Server Operation - gripperRotation_server
//# Start gripperRotation_operation Marker
bool servo_controller::gripperRotation_operation(agse_package::gripperRotation::Request  &req,
  agse_package::gripperRotation::Response &res)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering servo_controller::gripperRotation_operation");
#endif
  // Business Logic for gripperRotation_server_operation

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting servo_controller::gripperRotation_operation");
#endif
  return true;
}
//# End gripperRotation_operation Marker

// Timer Callback - servoTimer
//# Start servoTimer_operation Marker
void servo_controller::servoTimer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering servo_controller::servoTimer_operation");
#endif
  // Business Logic for servoTimer_operation

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting servo_controller::servoTimer_operation");
#endif
}
//# End servoTimer_operation Marker


// Destructor - Cleanup Ports & Timers
servo_controller::~servo_controller()
{
  servoTimer.stop();
  controlInputs_sub.shutdown();
  armRotation_server.shutdown();
  gripperPos_server.shutdown();
  gripperRotation_server.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void servo_controller::startUp()
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
  callback_options.alias = "armRotation_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec =0;
  callback_options.deadline.nsec = 100000000;
#endif    
  // Component Server - armRotation_server
  advertiseName = "armRotation";
  if (config.portGroupMap.find("armRotation_server") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["armRotation_server"];
  NAMESPACE::AdvertiseServiceOptions armRotation_server_server_options;
  armRotation_server_server_options = NAMESPACE::AdvertiseServiceOptions::create<agse_package::armRotation>
      (advertiseName.c_str(),
       boost::bind(&servo_controller::armRotation_operation, this, _1, _2),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD       
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif
  this->armRotation_server = nh.advertiseService(armRotation_server_server_options);
#ifdef USE_ROSMOD  
  callback_options.alias = "gripperPos_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec =0;
  callback_options.deadline.nsec = 100000000;
#endif    
  // Component Server - gripperPos_server
  advertiseName = "gripperPos";
  if (config.portGroupMap.find("gripperPos_server") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["gripperPos_server"];
  NAMESPACE::AdvertiseServiceOptions gripperPos_server_server_options;
  gripperPos_server_server_options = NAMESPACE::AdvertiseServiceOptions::create<agse_package::gripperPos>
      (advertiseName.c_str(),
       boost::bind(&servo_controller::gripperPos_operation, this, _1, _2),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD       
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif
  this->gripperPos_server = nh.advertiseService(gripperPos_server_server_options);
#ifdef USE_ROSMOD  
  callback_options.alias = "gripperRotation_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec =0;
  callback_options.deadline.nsec = 100000000;
#endif    
  // Component Server - gripperRotation_server
  advertiseName = "gripperRotation";
  if (config.portGroupMap.find("gripperRotation_server") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["gripperRotation_server"];
  NAMESPACE::AdvertiseServiceOptions gripperRotation_server_server_options;
  gripperRotation_server_server_options = NAMESPACE::AdvertiseServiceOptions::create<agse_package::gripperRotation>
      (advertiseName.c_str(),
       boost::bind(&servo_controller::gripperRotation_operation, this, _1, _2),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD       
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif
  this->gripperRotation_server = nh.advertiseService(gripperRotation_server_server_options);

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
	 boost::bind(&servo_controller::component_sync_operation, this, _1),
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
       boost::bind(&servo_controller::controlInputs_sub_operation, this, _1),
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
     boost::bind(&servo_controller::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();
#ifdef USE_ROSMOD   
  callback_options.alias = "servoTimer_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif
  // Component Timer - servoTimer
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(1.0),
     boost::bind(&servo_controller::servoTimer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->servoTimer = nh.createTimer(timer_options);


  this->init_timer.start();
  this->servoTimer.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new servo_controller(config,argc,argv);
  }
}
