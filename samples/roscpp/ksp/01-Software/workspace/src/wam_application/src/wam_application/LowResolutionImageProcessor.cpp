#include "wam_application/LowResolutionImageProcessor.hpp"


//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void LowResolutionImageProcessor::Init(const ros::TimerEvent& event)
{
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Timer Callback - LRImage_Timer
//# Start LRImage_TimerCallback Marker
void LowResolutionImageProcessor::LRImage_TimerCallback(const ros::TimerEvent& event)
{
  // Business Logic for LRImage_Timer Timer
}
//# End LRImage_TimerCallback Marker


// Destructor - Cleanup Ports & Timers
LowResolutionImageProcessor::~LowResolutionImageProcessor()
{
  LRImage_Timer.stop();
  LRImage_Publisher.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void LowResolutionImageProcessor::startUp()
{
  LOGGER.DEBUG("Entering LowResolutionImageProcessor::startUp");
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Publisher - LRImage_Publisher
  advertiseName = "LRImageVector";
  if (portGroupMap.find("LRImage_Publisher") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["LRImage_Publisher"];
  this->LRImage_Publisher = nh.advertise<wam_application::LRImageVector>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&LowResolutionImageProcessor::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - LRImage_Timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(5.0),
     boost::bind(&LowResolutionImageProcessor::LRImage_TimerCallback, this, _1),
     &this->compQueue);
  this->LRImage_Timer = nh.createTimer(timer_options);

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

  LOGGER.DEBUG("Exiting LowResolutionImageProcessor::startUp");
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new LowResolutionImageProcessor(config,argc,argv);
  }
}
