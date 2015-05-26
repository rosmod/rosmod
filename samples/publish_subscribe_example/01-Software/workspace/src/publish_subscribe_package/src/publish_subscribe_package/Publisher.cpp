#include "publish_subscribe_package/Publisher.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Publisher::Init(const ros::TimerEvent& event)
{
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Timer Callback - publish_timer
//# Start publish_timerCallback Marker
void Publisher::publish_timerCallback(const ros::TimerEvent& event)
{
  // Business Logic for publish_timer Timer
  publish_subscribe_package::Message message_;
  message_.name = "Publisher";
  publisher_port.publish(message_);
  LOGGER.INFO("Publisher::Published on Message topic!");
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
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Publisher - publisher_port
  advertiseName = "Message";
  if (portGroupMap.find("publisher_port") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["publisher_port"];
  this->publisher_port = nh.advertise<publish_subscribe_package::Message>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Publisher::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - timer.properties["name"]
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.5),
     boost::bind(&Publisher::publish_timerCallback, this, _1),
     &this->compQueue);
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
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new Publisher(config,argc,argv);
  }
}
