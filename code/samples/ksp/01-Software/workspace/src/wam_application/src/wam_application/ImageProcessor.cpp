#include "wam_application/ImageProcessor.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void ImageProcessor::Init(const ros::TimerEvent& event)
{
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Subscriber Callback - HRImage_Subscriber
//# Start HRImage_Subscriber_OnOneData Marker
void ImageProcessor::HRImage_Subscriber_OnOneData(const wam_application::HRImageVector::ConstPtr& received_data)
{
  // Business Logic for HRImage_Subscriber Subscriber
}
//# End HRImage_Subscriber_OnOneData Marker
// Subscriber Callback - LRImage_Subscriber
//# Start LRImage_Subscriber_OnOneData Marker
void ImageProcessor::LRImage_Subscriber_OnOneData(const wam_application::LRImageVector::ConstPtr& received_data)
{
  // Business Logic for LRImage_Subscriber Subscriber
}
//# End LRImage_Subscriber_OnOneData Marker


// Destructor - Cleanup Ports & Timers
ImageProcessor::~ImageProcessor()
{
  HRImage_Subscriber.shutdown();
  LRImage_Subscriber.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void ImageProcessor::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Subscriber - HRImage_Subscriber
  advertiseName = "HRImageVector";
  if (portGroupMap.find("HRImage_Subscriber") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["HRImage_Subscriber"];
  ros::SubscribeOptions HRImage_Subscriber_options;
  HRImage_Subscriber_options = ros::SubscribeOptions::create<wam_application::HRImageVector>
      (advertiseName.c_str(),
       1000,
       boost::bind(&ImageProcessor::HRImage_Subscriber_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->HRImage_Subscriber = nh.subscribe(HRImage_Subscriber_options);
  // Component Subscriber - LRImage_Subscriber
  advertiseName = "LRImageVector";
  if (portGroupMap.find("LRImage_Subscriber") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["LRImage_Subscriber"];
  ros::SubscribeOptions LRImage_Subscriber_options;
  LRImage_Subscriber_options = ros::SubscribeOptions::create<wam_application::LRImageVector>
      (advertiseName.c_str(),
       1000,
       boost::bind(&ImageProcessor::LRImage_Subscriber_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->LRImage_Subscriber = nh.subscribe(LRImage_Subscriber_options);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&ImageProcessor::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
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
    return new ImageProcessor(config,argc,argv);
  }
}
