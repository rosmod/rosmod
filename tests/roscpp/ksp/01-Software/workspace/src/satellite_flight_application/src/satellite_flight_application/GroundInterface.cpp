#include "satellite_flight_application/GroundInterface.hpp"


//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void GroundInterface::Init(const ros::TimerEvent& event)
{
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Timer Callback - GroundInterface_Timer
//# Start GroundInterface_TimerCallback Marker
void GroundInterface::GroundInterface_TimerCallback(const ros::TimerEvent& event)
{
  // Business Logic for GroundInterface_Timer Timer
}
//# End GroundInterface_TimerCallback Marker


// Destructor - Cleanup Ports & Timers
GroundInterface::~GroundInterface()
{
  GroundInterface_Timer.stop();
  GroundCommand_Publisher.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void GroundInterface::startUp()
{
  LOGGER.DEBUG("Entering GroundInterface::startUp");
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Publisher - GroundCommand_Publisher
  advertiseName = "GroundCommand";
  if (portGroupMap.find("GroundCommand_Publisher") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["GroundCommand_Publisher"];
  this->GroundCommand_Publisher = nh.advertise<satellite_flight_application::GroundCommand>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&GroundInterface::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - GroundInterface_Timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(1.0),
     boost::bind(&GroundInterface::GroundInterface_TimerCallback, this, _1),
     &this->compQueue);
  this->GroundInterface_Timer = nh.createTimer(timer_options);

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

  LOGGER.DEBUG("Exiting GroundInterface::startUp");
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new GroundInterface(config,argc,argv);
  }
}
