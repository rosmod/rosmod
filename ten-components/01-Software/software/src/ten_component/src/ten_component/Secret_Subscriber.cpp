#include "ten_component/Secret_Subscriber.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Secret_Subscriber::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering Secret_Subscriber::init_timer_operation");
#endif
  // Initialize Here
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting Secret_Subscriber::init_timer_operation");
#endif  
}
//# End Init Marker



// Subscriber Operation - secret_message_subscriber
//# Start secret_message_subscriber_operation Marker
#pragma optimize( "", off )
void Secret_Subscriber::secret_message_subscriber_operation(const ten_component::secret_message::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering Secret_Subscriber::secret_message_subscriber_operation");
#endif
  // Business Logic for secret_message_subscriber_operation
  for(int i=0; i < 2000000; i++) {
    double result = 0.0;
    double x = 41865185131.214415;
    double y = 562056205.1515;
    result = x*y;
  }
  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting Secret_Subscriber::secret_message_subscriber_operation");
#endif
}
#pragma optimize( "", on )
//# End secret_message_subscriber_operation Marker

// Server Operation - secret_server
//# Start secret_service_operation Marker
#pragma optimize( "", off )
bool Secret_Subscriber::secret_service_operation(ten_component::secret_service::Request  &req,
  ten_component::secret_service::Response &res)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering Secret_Subscriber::secret_service_operation");
#endif
  // Business Logic for secret_server_operation
  for(int i=0; i < 500000; i++) {
    double result = 0.0;
    double x = 41865185131.214415;
    double y = 562056205.1515;
    result = x*y;
  }
  res.command = "Wait for TANGO";
  res.calibration = 5.235062062;
  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting Secret_Subscriber::secret_service_operation");
#endif
  return true;
}
#pragma optimize( "", on )
//# End secret_service_operation Marker


// Destructor - Cleanup Ports & Timers
Secret_Subscriber::~Secret_Subscriber()
{
  secret_message_subscriber.shutdown();
  secret_server.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Secret_Subscriber::startUp()
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
  // Configure all provided services associated with this component
#ifdef USE_ROSMOD  
  callback_options.alias = "secret_service_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec =0;
  callback_options.deadline.nsec = 75000000;
#endif    
  // Component Server - secret_server
  advertiseName = "secret_service";
  if (config.portGroupMap.find("secret_server") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["secret_server"];
  NAMESPACE::AdvertiseServiceOptions secret_server_server_options;
  secret_server_server_options = NAMESPACE::AdvertiseServiceOptions::create<ten_component::secret_service>
      (advertiseName.c_str(),
       boost::bind(&Secret_Subscriber::secret_service_operation, this, _1, _2),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD       
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif
  this->secret_server = nh.advertiseService(secret_server_server_options);

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
     boost::bind(&Secret_Subscriber::component_sync_operation, this, _1),
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

  // Configure all subscribers associated with this component
#ifdef USE_ROSMOD 
  callback_options.alias = "secret_message_subscriber_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 200000000;
#endif  
  // Component Subscriber - secret_message_subscriber
  advertiseName = "secret_message";
  if (config.portGroupMap.find("secret_message_subscriber") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["secret_message_subscriber"];
  NAMESPACE::SubscribeOptions secret_message_subscriber_options;
  secret_message_subscriber_options = NAMESPACE::SubscribeOptions::create<ten_component::secret_message>
      (advertiseName.c_str(),
       1000,
       boost::bind(&Secret_Subscriber::secret_message_subscriber_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  this->secret_message_subscriber = nh.subscribe(secret_message_subscriber_options);

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
     boost::bind(&Secret_Subscriber::init_timer_operation, this, _1),
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
    return new Secret_Subscriber(config,argc,argv);
  }
}
