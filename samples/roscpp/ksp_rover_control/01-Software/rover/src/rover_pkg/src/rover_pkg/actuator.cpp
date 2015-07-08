#include "rover_pkg/actuator.hpp"

KRPCI krpci_client;

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void actuator::Init(const ros::TimerEvent& event)
{
  LOGGER.DEBUG("Entering actuator::Init");
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
  LOGGER.DEBUG("Exiting actuator::Init");
}
//# End Init Marker

// Subscriber Callback - control_command_sub
//# Start control_command_sub_OnOneData Marker
void actuator::control_command_sub_OnOneData(const rover_pkg::control_command::ConstPtr& received_data)
{
  LOGGER.DEBUG("Entering actuator::control_command_sub_OnOneData");
  // Business Logic for control_command_sub Subscriber

  LOGGER.DEBUG("Exiting actuator::control_command_sub_OnOneData");
}
//# End control_command_sub_OnOneData Marker


// Destructor - Cleanup Ports & Timers
actuator::~actuator()
{
  control_command_sub.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void actuator::startUp()
{
  LOGGER.DEBUG("Entering actuator::startUp");
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Subscriber - control_command_sub
  advertiseName = "control_command";
  if (portGroupMap.find("control_command_sub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["control_command_sub"];
  ros::SubscribeOptions control_command_sub_options;
  control_command_sub_options = ros::SubscribeOptions::create<rover_pkg::control_command>
      (advertiseName.c_str(),
       1000,
       boost::bind(&actuator::control_command_sub_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->control_command_sub = nh.subscribe(control_command_sub_options);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&actuator::Init, this, _1),
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
  LOGGER.DEBUG("Exiting actuator::startUp");
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new actuator(config,argc,argv);
  }
}
