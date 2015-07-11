#include "wam_application/HighResolutionImageProcessor.hpp"


//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void HighResolutionImageProcessor::Init(const ros::TimerEvent& event)
{
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Timer Callback - HRImage_Timer
//# Start HRImage_TimerCallback Marker
void HighResolutionImageProcessor::HRImage_TimerCallback(const ros::TimerEvent& event)
{
  // Business Logic for HRImage_Timer Timer
}
//# End HRImage_TimerCallback Marker


// Destructor - Cleanup Ports & Timers
HighResolutionImageProcessor::~HighResolutionImageProcessor()
{
  HRImage_Timer.stop();
  HRImage_Publisher.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void HighResolutionImageProcessor::startUp()
{
  LOGGER.DEBUG("Entering HighResolutionImageProcessor::startUp");
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Publisher - HRImage_Publisher
  advertiseName = "HRImageVector";
  if (portGroupMap.find("HRImage_Publisher") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["HRImage_Publisher"];
  this->HRImage_Publisher = nh.advertise<wam_application::HRImageVector>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&HighResolutionImageProcessor::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - HRImage_Timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(10.0),
     boost::bind(&HighResolutionImageProcessor::HRImage_TimerCallback, this, _1),
     &this->compQueue);
  this->HRImage_Timer = nh.createTimer(timer_options);

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

  LOGGER.DEBUG("Exiting HighResolutionImageProcessor::startUp");
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new HighResolutionImageProcessor(config,argc,argv);
  }
}
