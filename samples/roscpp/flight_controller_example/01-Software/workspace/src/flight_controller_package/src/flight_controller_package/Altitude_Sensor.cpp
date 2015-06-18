#include "flight_controller_package/Altitude_Sensor.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Altitude_Sensor::Init(const ros::TimerEvent& event)
{
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Timer Callback - altitude_sensor_timer
//# Start altitude_sensor_timerCallback Marker
void Altitude_Sensor::altitude_sensor_timerCallback(const ros::TimerEvent& event)
{
  // Business Logic for altitude_sensor_timer Timer
}
//# End altitude_sensor_timerCallback Marker


// Destructor - Cleanup Ports & Timers
Altitude_Sensor::~Altitude_Sensor()
{
  altitude_sensor_timer.stop();
  altitude_publisher.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Altitude_Sensor::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Publisher - altitude_publisher
  advertiseName = "Pitch";
  if (portGroupMap.find("altitude_publisher") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["altitude_publisher"];
  this->altitude_publisher = nh.advertise<flight_controller_package::Pitch>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Altitude_Sensor::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - altitude_sensor_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.2),
     boost::bind(&Altitude_Sensor::altitude_sensor_timerCallback, this, _1),
     &this->compQueue);
  this->altitude_sensor_timer = nh.createTimer(timer_options);

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
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new Altitude_Sensor(config,argc,argv);
  }
}
