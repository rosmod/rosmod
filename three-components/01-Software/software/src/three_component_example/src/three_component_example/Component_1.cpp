#include "three_component_example/Component_1.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Component_1::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering Component_1::init_timer_operation");
#endif
  // Initialize Here
  three_component_example::ComponentName compName;
  compName.name = "Component_1";
  logger->log("INFO", 
	      "Component_1::Name_Publisher::Publishing Component Name::%s", compName.name.c_str());
  Name_Publisher.publish(compName);
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting Component_1::init_timer_operation");
#endif  
}
//# End Init Marker

// Subscriber Operation - Name_Subscriber
//# Start Name_Subscriber_operation Marker
void Component_1::Name_Subscriber_operation(const three_component_example::ComponentName::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering Component_1::Name_Subscriber_operation");
#endif
  // Business Logic for Name_Subscriber_operation
  three_component_example::ComponentName compName;
  compName.name = "Component_1";
  logger->log("INFO", 
	      "Component_1::Name_Subscriber::Received Component Name::%s", compName.name.c_str());
  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting Component_1::Name_Subscriber_operation");
#endif
}
//# End Name_Subscriber_operation Marker

// Timer Callback - Timer_1
//# Start Timer_1_operation Marker
void Component_1::Timer_1_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering Component_1::Timer_1_operation");
#endif
  // Business Logic for Timer_1_operation
  logger->log("INFO", "Component_1::Timer_1::Timer Callback has been triggered!");
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting Component_1::Timer_1_operation");
#endif
}
//# End Timer_1_operation Marker


// Destructor - Cleanup Ports & Timers
Component_1::~Component_1()
{
  Timer_1.stop();
  Name_Publisher.shutdown();
  Name_Subscriber.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Component_1::startUp()
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
  // Component Publisher - Name_Publisher
  advertiseName = "ComponentName";
  if (config.portGroupMap.find("Name_Publisher") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["Name_Publisher"];
  this->Name_Publisher = nh.advertise<three_component_example::ComponentName>(advertiseName.c_str(), 1000);


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
     boost::bind(&Component_1::component_sync_operation, this, _1),
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
  callback_options.alias = "Name_Subscriber_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 300000000;
#endif  
  // Component Subscriber - Name_Subscriber
  advertiseName = "ComponentName";
  if (config.portGroupMap.find("Name_Subscriber") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["Name_Subscriber"];
  NAMESPACE::SubscribeOptions Name_Subscriber_options;
  Name_Subscriber_options = NAMESPACE::SubscribeOptions::create<three_component_example::ComponentName>
      (advertiseName.c_str(),
       1000,
       boost::bind(&Component_1::Name_Subscriber_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  this->Name_Subscriber = nh.subscribe(Name_Subscriber_options);

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
     boost::bind(&Component_1::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();
#ifdef USE_ROSMOD   
  callback_options.alias = "Timer_1_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 200000000;
#endif
  // Component Timer - Timer_1
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(0.5),
     boost::bind(&Component_1::Timer_1_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->Timer_1 = nh.createTimer(timer_options);


  this->init_timer.start();
  this->Timer_1.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new Component_1(config,argc,argv);
  }
}
