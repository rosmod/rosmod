#include "publish_subscribe_package/Publisher.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Publisher::Init(const rosmod::TimerEvent& event)
{
  compQueue.ROSMOD_LOGGER.DEBUG("Entering Timer::Init");
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
  compQueue.ROSMOD_LOGGER.DEBUG("Exiting Timer::Init");  
}
//# End Init Marker

// Timer Callback - publish_timer
//# Start publish_timerCallback Marker
void Publisher::publish_timerCallback(const rosmod::TimerEvent& event)
{
  compQueue.ROSMOD_LOGGER.DEBUG("Entering publish_timerCallback");  
  // Business Logic for publish_timer Timer
  publish_subscribe_package::Message message_;
  message_.name = "Publisher";
  publisher_port.publish(message_);
  LOGGER.INFO("Publisher::Published on Message topic!");
  compQueue.ROSMOD_LOGGER.DEBUG("Exiting publish_timerCallback");  
}
//# End publish_timerCallback Marker


// Destructor - Cleanup Ports & Timers
Publisher::~Publisher()
{
  publish_timer.stop();
  publisher_port.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Publisher::startUp()
{
  rosmod::NodeHandle nh;
  std::string advertiseName;

  // Scheduling Scheme is FIFO
  this->compQueue.scheduling_scheme = rosmod::CallbackQueue::SchedulingScheme::FIFO;

  rosmod::ROSMOD_Callback_Options callback_options;
  callback_options.alias = "Init_Timer";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0.0;
  callback_options.deadline.nsec = 40000;
  
  // Component Publisher - publisher_port
  advertiseName = "Message";
  if (portGroupMap.find("publisher_port") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["publisher_port"];
  this->publisher_port = nh.advertise<publish_subscribe_package::Message>(advertiseName.c_str(), 1000);

  // Init Timer
  rosmod::TimerOptions timer_options;
  timer_options = 
    rosmod::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Publisher::Init, this, _1),
     &this->compQueue,
     callback_options,
     true,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);

  callback_options.alias = "Publish_Timer";
  
  // Component Timer - publish_timer
  timer_options = 
    rosmod::TimerOptions
    (ros::Duration(0.5),
     boost::bind(&Publisher::publish_timerCallback, this, _1),
     &this->compQueue,
     callback_options,
     false,
     true);
  this->publish_timer = nh.createTimer(timer_options);

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
  std::string log_file_path = pwd + nodeName + "." + compName + ".log"; 
  
  // Create the log file & open file stream
  LOGGER.CREATE_FILE(log_file_path);
  
  // Establish log levels of LOGGER
  LOGGER.SET_LOG_LEVELS(logLevels);

  compQueue.ROSMOD_LOGGER.CREATE_FILE(pwd + "ROSMOD_DEBUG." + nodeName + "." + compName + ".log");
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new Publisher(config,argc,argv);
  }
}
