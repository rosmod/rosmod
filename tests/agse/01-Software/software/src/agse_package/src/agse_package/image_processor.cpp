#include "agse_package/image_processor.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void image_processor::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering image_processor::init_timer_operation");
#endif
  // Initialize Here
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting image_processor::init_timer_operation");
#endif  
}
//# End Init Marker



// Subscriber Operation - controlInputs_sub
//# Start controlInputs_sub_operation Marker
void image_processor::controlInputs_sub_operation(const agse_package::controlInputs::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering image_processor::controlInputs_sub_operation");
#endif
  // Business Logic for controlInputs_sub_operation

  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting image_processor::controlInputs_sub_operation");
#endif
}
//# End controlInputs_sub_operation Marker

// Server Operation - sampleStateFromImage_server
//# Start sampleStateFromImage_operation Marker
bool image_processor::sampleStateFromImage_operation(agse_package::sampleStateFromImage::Request  &req,
  agse_package::sampleStateFromImage::Response &res)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering image_processor::sampleStateFromImage_operation");
#endif
  // Business Logic for sampleStateFromImage_server_operation

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting image_processor::sampleStateFromImage_operation");
#endif
  return true;
}
//# End sampleStateFromImage_operation Marker
// Server Operation - payloadBayStateFromImage_server
//# Start payloadBayStateFromImage_operation Marker
bool image_processor::payloadBayStateFromImage_operation(agse_package::payloadBayStateFromImage::Request  &req,
  agse_package::payloadBayStateFromImage::Response &res)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering image_processor::payloadBayStateFromImage_operation");
#endif
  // Business Logic for payloadBayStateFromImage_server_operation

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting image_processor::payloadBayStateFromImage_operation");
#endif
  return true;
}
//# End payloadBayStateFromImage_operation Marker


// Destructor - Cleanup Ports & Timers
image_processor::~image_processor()
{
  payloadBayDetectionImages_pub.shutdown();
  sampleDetectionImages_pub.shutdown();
  controlInputs_sub.shutdown();
  sampleStateFromImage_server.shutdown();
  payloadBayStateFromImage_server.shutdown();
  captureImage_client.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void image_processor::startUp()
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
  // Component Publisher - payloadBayDetectionImages_pub
  advertiseName = "payloadBayDetectionImages";
  if (config.portGroupMap.find("payloadBayDetectionImages_pub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["payloadBayDetectionImages_pub"];
  this->payloadBayDetectionImages_pub = nh.advertise<agse_package::payloadBayDetectionImages>(advertiseName.c_str(), 1000);
  // Component Publisher - sampleDetectionImages_pub
  advertiseName = "sampleDetectionImages";
  if (config.portGroupMap.find("sampleDetectionImages_pub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["sampleDetectionImages_pub"];
  this->sampleDetectionImages_pub = nh.advertise<agse_package::sampleDetectionImages>(advertiseName.c_str(), 1000);

  // Configure all provided services associated with this component
#ifdef USE_ROSMOD  
  callback_options.alias = "sampleStateFromImage_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec =0;
  callback_options.deadline.nsec = 100000000;
#endif    
  // Component Server - sampleStateFromImage_server
  advertiseName = "sampleStateFromImage";
  if (config.portGroupMap.find("sampleStateFromImage_server") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["sampleStateFromImage_server"];
  NAMESPACE::AdvertiseServiceOptions sampleStateFromImage_server_server_options;
  sampleStateFromImage_server_server_options = NAMESPACE::AdvertiseServiceOptions::create<agse_package::sampleStateFromImage>
      (advertiseName.c_str(),
       boost::bind(&image_processor::sampleStateFromImage_operation, this, _1, _2),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD       
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif
  this->sampleStateFromImage_server = nh.advertiseService(sampleStateFromImage_server_server_options);
#ifdef USE_ROSMOD  
  callback_options.alias = "payloadBayStateFromImage_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec =0;
  callback_options.deadline.nsec = 100000000;
#endif    
  // Component Server - payloadBayStateFromImage_server
  advertiseName = "payloadBayStateFromImage";
  if (config.portGroupMap.find("payloadBayStateFromImage_server") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["payloadBayStateFromImage_server"];
  NAMESPACE::AdvertiseServiceOptions payloadBayStateFromImage_server_server_options;
  payloadBayStateFromImage_server_server_options = NAMESPACE::AdvertiseServiceOptions::create<agse_package::payloadBayStateFromImage>
      (advertiseName.c_str(),
       boost::bind(&image_processor::payloadBayStateFromImage_operation, this, _1, _2),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD       
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif
  this->payloadBayStateFromImage_server = nh.advertiseService(payloadBayStateFromImage_server_server_options);

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
	 boost::bind(&image_processor::component_sync_operation, this, _1),
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
       boost::bind(&image_processor::controlInputs_sub_operation, this, _1),
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
     boost::bind(&image_processor::init_timer_operation, this, _1),
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
    return new image_processor(config,argc,argv);
  }
}
