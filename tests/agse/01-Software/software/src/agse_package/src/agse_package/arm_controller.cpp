#include "agse_package/arm_controller.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void arm_controller::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering arm_controller::init_timer_operation");
#endif
  // Initialize Here
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting arm_controller::init_timer_operation");
#endif  
}
//# End Init Marker



// Subscriber Operation - controlInputs_sub
//# Start controlInputs_sub_operation Marker
void arm_controller::controlInputs_sub_operation(const agse_package::controlInputs::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering arm_controller::controlInputs_sub_operation");
#endif
  // Business Logic for controlInputs_sub_operation

  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting arm_controller::controlInputs_sub_operation");
#endif
}
//# End controlInputs_sub_operation Marker

// Timer Callback - armTimer
//# Start armTimer_operation Marker
void arm_controller::armTimer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering arm_controller::armTimer_operation");
#endif
  // Business Logic for armTimer_operation

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting arm_controller::armTimer_operation");
#endif
}
//# End armTimer_operation Marker


// Destructor - Cleanup Ports & Timers
arm_controller::~arm_controller()
{
  armTimer.stop();
  sampleState_pub.shutdown();
  payloadBayState_pub.shutdown();
  armState_pub.shutdown();
  controlInputs_sub.shutdown();
  sampleStateFromImage_client.shutdown();
  radialPos_client.shutdown();
  armRotation_client.shutdown();
  gripperRotation_client.shutdown();
  verticalPos_client.shutdown();
  gripperPos_client.shutdown();
  payloadBayStateFromImage_client.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void arm_controller::startUp()
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
  // Component Publisher - sampleState_pub
  advertiseName = "sampleState";
  if (config.portGroupMap.find("sampleState_pub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["sampleState_pub"];
  this->sampleState_pub = nh.advertise<agse_package::sampleState>(advertiseName.c_str(), 1000);
  // Component Publisher - payloadBayState_pub
  advertiseName = "payloadBayState";
  if (config.portGroupMap.find("payloadBayState_pub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["payloadBayState_pub"];
  this->payloadBayState_pub = nh.advertise<agse_package::payloadBayState>(advertiseName.c_str(), 1000);
  // Component Publisher - armState_pub
  advertiseName = "armState";
  if (config.portGroupMap.find("armState_pub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["armState_pub"];
  this->armState_pub = nh.advertise<agse_package::armState>(advertiseName.c_str(), 1000);


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
	 boost::bind(&arm_controller::component_sync_operation, this, _1),
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
       boost::bind(&arm_controller::controlInputs_sub_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  controlInputs_sub_options.transport_hints = NAMESPACE::TransportHints().udp();
  this->controlInputs_sub = nh.subscribe(controlInputs_sub_options);

  // Configure all required services associated with this component
  // Component Client - sampleStateFromImage_client
  advertiseName = "sampleStateFromImage";
  if (config.portGroupMap.find("sampleStateFromImage_client") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["sampleStateFromImage_client"];
  this->sampleStateFromImage_client = nh.serviceClient<agse_package::sampleStateFromImage>(advertiseName.c_str(), true); 
  // Component Client - radialPos_client
  advertiseName = "radialPos";
  if (config.portGroupMap.find("radialPos_client") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["radialPos_client"];
  this->radialPos_client = nh.serviceClient<agse_package::radialPos>(advertiseName.c_str(), true); 
  // Component Client - armRotation_client
  advertiseName = "armRotation";
  if (config.portGroupMap.find("armRotation_client") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["armRotation_client"];
  this->armRotation_client = nh.serviceClient<agse_package::armRotation>(advertiseName.c_str(), true); 
  // Component Client - gripperRotation_client
  advertiseName = "gripperRotation";
  if (config.portGroupMap.find("gripperRotation_client") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["gripperRotation_client"];
  this->gripperRotation_client = nh.serviceClient<agse_package::gripperRotation>(advertiseName.c_str(), true); 
  // Component Client - verticalPos_client
  advertiseName = "verticalPos";
  if (config.portGroupMap.find("verticalPos_client") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["verticalPos_client"];
  this->verticalPos_client = nh.serviceClient<agse_package::verticalPos>(advertiseName.c_str(), true); 
  // Component Client - gripperPos_client
  advertiseName = "gripperPos";
  if (config.portGroupMap.find("gripperPos_client") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["gripperPos_client"];
  this->gripperPos_client = nh.serviceClient<agse_package::gripperPos>(advertiseName.c_str(), true); 
  // Component Client - payloadBayStateFromImage_client
  advertiseName = "payloadBayStateFromImage";
  if (config.portGroupMap.find("payloadBayStateFromImage_client") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["payloadBayStateFromImage_client"];
  this->payloadBayStateFromImage_client = nh.serviceClient<agse_package::payloadBayStateFromImage>(advertiseName.c_str(), true); 

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
     boost::bind(&arm_controller::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();
#ifdef USE_ROSMOD   
  callback_options.alias = "armTimer_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif
  // Component Timer - armTimer
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(0.2),
     boost::bind(&arm_controller::armTimer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->armTimer = nh.createTimer(timer_options);


  this->init_timer.start();
  this->armTimer.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new arm_controller(config,argc,argv);
  }
}
