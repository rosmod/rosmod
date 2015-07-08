#include "rover_pkg/sensor.hpp"

KRPCI krpci_client;

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void sensor::Init(const ros::TimerEvent& event)
{
  LOGGER.DEBUG("Entering sensor::Init");
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
  LOGGER.DEBUG("Exiting sensor::Init");
}
//# End Init Marker

// Timer Callback - sensor_timer
//# Start sensor_timerCallback Marker
void sensor::sensor_timerCallback(const ros::TimerEvent& event)
{
  LOGGER.DEBUG("Entering sensor::sensor_timerCallback");
  // Business Logic for sensor_timer Timer

  LOGGER.DEBUG("Exiting sensor::sensor_timerCallback");
}
//# End sensor_timerCallback Marker


// Destructor - Cleanup Ports & Timers
sensor::~sensor()
{
  sensor_timer.stop();
  vessel_state_pub.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void sensor::startUp()
{
  LOGGER.DEBUG("Entering sensor::startUp");
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Publisher - vessel_state_pub
  advertiseName = "vessel_state";
  if (portGroupMap.find("vessel_state_pub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["vessel_state_pub"];
  this->vessel_state_pub = nh.advertise<rover_pkg::vessel_state>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&sensor::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - sensor_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.1),
     boost::bind(&sensor::sensor_timerCallback, this, _1),
     &this->compQueue);
  this->sensor_timer = nh.createTimer(timer_options);

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

  krpci_client.SetName(nodeName + "_" + compName);
  LOGGER.DEBUG("Exiting sensor::startUp");
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new sensor(config,argc,argv);
  }
}
