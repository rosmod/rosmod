#include "satellite_flight_application/SatelliteBusInterface.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void SatelliteBusInterface::Init(const ros::TimerEvent& event)
{
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Server Callback - SatelliteState_Server
//# Start SatelliteStateCallback Marker
bool SatelliteBusInterface::SatelliteStateCallback(satellite_flight_application::SatelliteState::Request  &req,
  satellite_flight_application::SatelliteState::Response &res)
{
  // Business Logic for SatelliteState_Server Server

  return true;
}
//# End SatelliteStateCallback Marker
// Server Callback - ThrusterComm_Server
//# Start ThrusterCommCallback Marker
bool SatelliteBusInterface::ThrusterCommCallback(satellite_flight_application::ThrusterComm::Request  &req,
  satellite_flight_application::ThrusterComm::Response &res)
{
  // Business Logic for ThrusterComm_Server Server

  return true;
}
//# End ThrusterCommCallback Marker


// Destructor - Cleanup Ports & Timers
SatelliteBusInterface::~SatelliteBusInterface()
{
  SatelliteState_Server.shutdown();
  ThrusterComm_Server.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void SatelliteBusInterface::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Server - SatelliteState_Server
  advertiseName = "SatelliteState";
  if (portGroupMap.find("SatelliteState_Server") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["SatelliteState_Server"];
  ros::AdvertiseServiceOptions SatelliteState_Server_server_options;
  SatelliteState_Server_server_options = ros::AdvertiseServiceOptions::create<satellite_flight_application::SatelliteState>
      (advertiseName.c_str(),
       boost::bind(&SatelliteBusInterface::SatelliteStateCallback, this, _1, _2),
       ros::VoidPtr(),
       &this->compQueue);
  this->SatelliteState_Server = nh.advertiseService(SatelliteState_Server_server_options);
  // Component Server - ThrusterComm_Server
  advertiseName = "ThrusterComm";
  if (portGroupMap.find("ThrusterComm_Server") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["ThrusterComm_Server"];
  ros::AdvertiseServiceOptions ThrusterComm_Server_server_options;
  ThrusterComm_Server_server_options = ros::AdvertiseServiceOptions::create<satellite_flight_application::ThrusterComm>
      (advertiseName.c_str(),
       boost::bind(&SatelliteBusInterface::ThrusterCommCallback, this, _1, _2),
       ros::VoidPtr(),
       &this->compQueue);
  this->ThrusterComm_Server = nh.advertiseService(ThrusterComm_Server_server_options);
 
  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&SatelliteBusInterface::Init, this, _1),
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
    return new SatelliteBusInterface(config,argc,argv);
  }
}
