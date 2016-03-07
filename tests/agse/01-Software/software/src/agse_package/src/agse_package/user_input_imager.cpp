#include "agse_package/user_input_imager.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void user_input_imager::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering user_input_imager::init_timer_operation");
#endif
  // Initialize Here
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting user_input_imager::init_timer_operation");
#endif  
}
//# End Init Marker



// Subscriber Operation - payloadBayDetectionImages_sub
//# Start payloadBayDetectionImages_sub_operation Marker
void user_input_imager::payloadBayDetectionImages_sub_operation(const agse_package::payloadBayDetectionImages::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering user_input_imager::payloadBayDetectionImages_sub_operation");
#endif
  // Business Logic for payloadBayDetectionImages_sub_operation

  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting user_input_imager::payloadBayDetectionImages_sub_operation");
#endif
}
//# End payloadBayDetectionImages_sub_operation Marker
// Subscriber Operation - sampleDetectionImages_sub
//# Start sampleDetectionImages_sub_operation Marker
void user_input_imager::sampleDetectionImages_sub_operation(const agse_package::sampleDetectionImages::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering user_input_imager::sampleDetectionImages_sub_operation");
#endif
  // Business Logic for sampleDetectionImages_sub_operation

  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting user_input_imager::sampleDetectionImages_sub_operation");
#endif
}
//# End sampleDetectionImages_sub_operation Marker
// Subscriber Operation - payloadBayState_sub
//# Start payloadBayState_sub_operation Marker
void user_input_imager::payloadBayState_sub_operation(const agse_package::payloadBayState::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering user_input_imager::payloadBayState_sub_operation");
#endif
  // Business Logic for payloadBayState_sub_operation

  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting user_input_imager::payloadBayState_sub_operation");
#endif
}
//# End payloadBayState_sub_operation Marker
// Subscriber Operation - sampleState_sub
//# Start sampleState_sub_operation Marker
void user_input_imager::sampleState_sub_operation(const agse_package::sampleState::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering user_input_imager::sampleState_sub_operation");
#endif
  // Business Logic for sampleState_sub_operation

  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting user_input_imager::sampleState_sub_operation");
#endif
}
//# End sampleState_sub_operation Marker

// Timer Callback - uiImage_timer
//# Start uiImage_timer_operation Marker
void user_input_imager::uiImage_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering user_input_imager::uiImage_timer_operation");
#endif
  // Business Logic for uiImage_timer_operation

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting user_input_imager::uiImage_timer_operation");
#endif
}
//# End uiImage_timer_operation Marker


// Destructor - Cleanup Ports & Timers
user_input_imager::~user_input_imager()
{
  uiImage_timer.stop();
  payloadBayDetectionImages_sub.shutdown();
  sampleDetectionImages_sub.shutdown();
  payloadBayState_sub.shutdown();
  sampleState_sub.shutdown();
  captureImage_client.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void user_input_imager::startUp()
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
	 boost::bind(&user_input_imager::component_sync_operation, this, _1),
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
  callback_options.alias = "payloadBayDetectionImages_sub_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif  
  // Component Subscriber - payloadBayDetectionImages_sub
  advertiseName = "payloadBayDetectionImages";
  if (config.portGroupMap.find("payloadBayDetectionImages_sub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["payloadBayDetectionImages_sub"];
  NAMESPACE::SubscribeOptions payloadBayDetectionImages_sub_options;
  payloadBayDetectionImages_sub_options = NAMESPACE::SubscribeOptions::create<agse_package::payloadBayDetectionImages>
      (advertiseName.c_str(),
       1000,
       boost::bind(&user_input_imager::payloadBayDetectionImages_sub_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  payloadBayDetectionImages_sub_options.transport_hints = NAMESPACE::TransportHints().udp();
  this->payloadBayDetectionImages_sub = nh.subscribe(payloadBayDetectionImages_sub_options);
#ifdef USE_ROSMOD 
  callback_options.alias = "sampleDetectionImages_sub_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif  
  // Component Subscriber - sampleDetectionImages_sub
  advertiseName = "sampleDetectionImages";
  if (config.portGroupMap.find("sampleDetectionImages_sub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["sampleDetectionImages_sub"];
  NAMESPACE::SubscribeOptions sampleDetectionImages_sub_options;
  sampleDetectionImages_sub_options = NAMESPACE::SubscribeOptions::create<agse_package::sampleDetectionImages>
      (advertiseName.c_str(),
       1000,
       boost::bind(&user_input_imager::sampleDetectionImages_sub_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  sampleDetectionImages_sub_options.transport_hints = NAMESPACE::TransportHints().udp();
  this->sampleDetectionImages_sub = nh.subscribe(sampleDetectionImages_sub_options);
#ifdef USE_ROSMOD 
  callback_options.alias = "payloadBayState_sub_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif  
  // Component Subscriber - payloadBayState_sub
  advertiseName = "payloadBayState";
  if (config.portGroupMap.find("payloadBayState_sub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["payloadBayState_sub"];
  NAMESPACE::SubscribeOptions payloadBayState_sub_options;
  payloadBayState_sub_options = NAMESPACE::SubscribeOptions::create<agse_package::payloadBayState>
      (advertiseName.c_str(),
       1000,
       boost::bind(&user_input_imager::payloadBayState_sub_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  payloadBayState_sub_options.transport_hints = NAMESPACE::TransportHints().udp();
  this->payloadBayState_sub = nh.subscribe(payloadBayState_sub_options);
#ifdef USE_ROSMOD 
  callback_options.alias = "sampleState_sub_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif  
  // Component Subscriber - sampleState_sub
  advertiseName = "sampleState";
  if (config.portGroupMap.find("sampleState_sub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["sampleState_sub"];
  NAMESPACE::SubscribeOptions sampleState_sub_options;
  sampleState_sub_options = NAMESPACE::SubscribeOptions::create<agse_package::sampleState>
      (advertiseName.c_str(),
       1000,
       boost::bind(&user_input_imager::sampleState_sub_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  sampleState_sub_options.transport_hints = NAMESPACE::TransportHints().udp();
  this->sampleState_sub = nh.subscribe(sampleState_sub_options);

  // Configure all required services associated with this component
  // Component Client - captureImage_client
  advertiseName = "captureImage";
  if (config.portGroupMap.find("captureImage_client") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["captureImage_client"];
  this->captureImage_client = nh.serviceClient<agse_package::captureImage>(advertiseName.c_str(), true); 

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
     boost::bind(&user_input_imager::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();
#ifdef USE_ROSMOD   
  callback_options.alias = "uiImage_timer_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif
  // Component Timer - uiImage_timer
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(0.5),
     boost::bind(&user_input_imager::uiImage_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->uiImage_timer = nh.createTimer(timer_options);


  this->init_timer.start();
  this->uiImage_timer.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new user_input_imager(config,argc,argv);
  }
}
