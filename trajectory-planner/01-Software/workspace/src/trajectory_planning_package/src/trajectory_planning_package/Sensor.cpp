#include "trajectory_planning_package/Sensor.hpp"

//# Start User Globals Marker
#include <math.h>
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Sensor::Init(const ros::TimerEvent& event)
{
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Server Callback - trajectory_server
//# Start computeCallback Marker
bool Sensor::computeCallback(trajectory_planning_package::compute::Request  &req,
  trajectory_planning_package::compute::Response &res)
{
  // Business Logic for trajectory_server Server
  LOGGER.INFO("Sensor::Server::Computing new result");
  float x_sq = pow(req.x, 2);
  float y_sq = pow(req.y, 2);
  float z_sq = pow(req.z, 2);
  float mean_sq = (x_sq + y_sq + z_sq)/3.0;
  res.result = sqrt(mean_sq);
  return true;
}
//# End computeCallback Marker

// Timer Callback - sensor_timer
//# Start sensor_timerCallback Marker
void Sensor::sensor_timerCallback(const ros::TimerEvent& event)
{
  // Business Logic for sensor_timer Timer
  trajectory_planning_package::sensor_reading reading;
  reading.x = 5.3452;
  reading.y = 7.5209;
  reading.z = 9.1525;
  reading.state = "ERROR";
  state_publisher.publish(reading);
  LOGGER.INFO("Sensor::Timer::Publishing current sensor state");
}
//# End sensor_timerCallback Marker


// Destructor - Cleanup Ports & Timers
Sensor::~Sensor()
{
  sensor_timer.stop();
  state_publisher.shutdown();
  trajectory_server.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Sensor::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Publisher - state_publisher
  advertiseName = "sensor_reading";
  if (portGroupMap.find("state_publisher") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["state_publisher"];
  this->state_publisher = nh.advertise<trajectory_planning_package::sensor_reading>(advertiseName.c_str(), 1000);

  // Component Server - trajectory_server
  advertiseName = "compute";
  if (portGroupMap.find("trajectory_server") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["trajectory_server"];
  ros::AdvertiseServiceOptions trajectory_server_server_options;
  trajectory_server_server_options = ros::AdvertiseServiceOptions::create<trajectory_planning_package::compute>
      (advertiseName.c_str(),
       boost::bind(&Sensor::computeCallback, this, _1, _2),
       ros::VoidPtr(),
       &this->compQueue);
  this->trajectory_server = nh.advertiseService(trajectory_server_server_options);
 
  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Sensor::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - sensor_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(1.0),
     boost::bind(&Sensor::sensor_timerCallback, this, _1),
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
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new Sensor(config,argc,argv);
  }
}
