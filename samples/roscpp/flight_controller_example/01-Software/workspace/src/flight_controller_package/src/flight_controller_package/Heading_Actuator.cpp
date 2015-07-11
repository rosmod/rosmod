#include "flight_controller_package/Heading_Actuator.hpp"

KRPCI krpci_client;

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Heading_Actuator::Init(const ros::TimerEvent& event)
{
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Server Callback - heading_control_server
//# Start Heading_ControlCallback Marker
bool Heading_Actuator::Heading_ControlCallback(flight_controller_package::Heading_Control::Request  &req,
  flight_controller_package::Heading_Control::Response &res)
{
  // Business Logic for heading_control_server Server

  return true;
}
//# End Heading_ControlCallback Marker


// Destructor - Cleanup Ports & Timers
Heading_Actuator::~Heading_Actuator()
{
  heading_control_server.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Heading_Actuator::startUp()
{
  LOGGER.DEBUG("Entering Heading_Actuator::startUp");
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Server - heading_control_server
  advertiseName = "Heading_Control";
  if (portGroupMap.find("heading_control_server") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["heading_control_server"];
  ros::AdvertiseServiceOptions heading_control_server_server_options;
  heading_control_server_server_options = ros::AdvertiseServiceOptions::create<flight_controller_package::Heading_Control>
      (advertiseName.c_str(),
       boost::bind(&Heading_Actuator::Heading_ControlCallback, this, _1, _2),
       ros::VoidPtr(),
       &this->compQueue);
  this->heading_control_server = nh.advertiseService(heading_control_server_server_options);
 
  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Heading_Actuator::Init, this, _1),
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

  krpci_client.SetName(nodeName + "_" + compName);
  LOGGER.DEBUG("Exiting Heading_Actuator::startUp");
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new Heading_Actuator(config,argc,argv);
  }
}
