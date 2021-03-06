#include "three_component_example/Component_1.hpp"


//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Component_1::Init(const NAMESPACE::TimerEvent& event)
{
  // Initialize Here
  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Subscriber Callback - Name_Subscriber
//# Start Name_Subscriber_OnOneData Marker
void Component_1::Name_Subscriber_OnOneData(const three_component_example::ComponentName::ConstPtr& received_data)
{
  // Business Logic for Name_Subscriber Subscriber

}
//# End Name_Subscriber_OnOneData Marker

// Timer Callback - Timer_1
//# Start Timer_1Callback Marker
void Component_1::Timer_1Callback(const NAMESPACE::TimerEvent& event)
{
  // Business Logic for Timer_1 Timer

}
//# End Timer_1Callback Marker


// Destructor - Cleanup Ports & Timers
Component_1::~Component_1()
{
  Timer_1.stop();
  Name_Publisher.shutdown();
  Name_Subscriber.shutdown();
  //# Start Destructor Marker
  //  delete compQueue.ROSMOD_LOGGER.release();
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

#ifdef USE_ROSMOD
  compQueue.ROSMOD_LOGGER->create_file(pwd + "ROSMOD_DEBUG." + config.nodeName + "." + config.compName + ".log");
  compQueue.ROSMOD_LOGGER->set_is_periodic(config.is_periodic_logging);
  compQueue.ROSMOD_LOGGER->set_max_log_unit(config.periodic_log_unit);
#endif    

#ifdef USE_ROSMOD
  // Scheduling Scheme is FIFO
  this->compQueue.scheduling_scheme = config.schedulingScheme;
  
  rosmod::ROSMOD_Callback_Options callback_options;
  callback_options.alias = "Name_Subscriber_OnOneData";
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
       boost::bind(&Component_1::Name_Subscriber_OnOneData, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->compQueue,
       callback_options);
#else
       &this->compQueue);
#endif  
  this->Name_Subscriber = nh.subscribe(Name_Subscriber_options);

  // Component Publisher - Name_Publisher
  advertiseName = "ComponentName";
  if (config.portGroupMap.find("Name_Publisher") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["Name_Publisher"];
  this->Name_Publisher = nh.advertise<three_component_example::ComponentName>(advertiseName.c_str(), 1000);

#ifdef USE_ROSMOD
  // Init Timer
  callback_options.alias = "Init_Timer";
  callback_options.priority = 99;
  callback_options.deadline.sec = 1;
  callback_options.deadline.nsec = 0;
  rosmod::TimerOptions timer_options;
#else
  ros::TimerOptions timer_options;
#endif

  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Component_1::Init, this, _1),
     &this->compQueue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false);
  this->initOneShotTimer = nh.createTimer(timer_options);
  this->initOneShotTimer.stop();

#ifdef USE_ROSMOD
  callback_options.alias = "Timer_1Callback";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 200000000;
#endif
  // Component Timer - Timer_1
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(0.5),
     boost::bind(&Component_1::Timer_1Callback, this, _1),
     &this->compQueue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     false,
     false);
  this->Timer_1 = nh.createTimer(timer_options);

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
     boost::bind(&Component_1::component_synchronization_OnOneData, this, _1),
     NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD     
     &this->compQueue,
     sync_callback_options);
#else
     &this->compQueue);
#endif     

  this->comp_sync_sub = nh.subscribe(comp_sync_sub_options);

  ros::Time now = ros::Time::now();
  while ( this->comp_sync_sub.getNumPublishers() < this->config.num_comps_to_sync &&
	  (ros::Time::now() - now) < ros::Duration(config.comp_sync_timeout) )
  ros::Duration(0.1).sleep();
  ros::Duration(0.5).sleep();
  this->comp_sync_sub.shutdown();
  this->comp_sync_pub.shutdown();

  this->initOneShotTimer.start();
  this->Timer_1.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new Component_1(config,argc,argv);
  }
}
