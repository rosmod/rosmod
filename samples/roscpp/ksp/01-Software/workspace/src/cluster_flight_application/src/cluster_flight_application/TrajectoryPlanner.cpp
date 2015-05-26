#include "cluster_flight_application/TrajectoryPlanner.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void TrajectoryPlanner::Init(const ros::TimerEvent& event)
{
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Subscriber Callback - SatState_Subscriber
//# Start SatState_Subscriber_OnOneData Marker
void TrajectoryPlanner::SatState_Subscriber_OnOneData(const satellite_flight_application::SatState::ConstPtr& received_data)
{
  // Business Logic for SatState_Subscriber Subscriber
}
//# End SatState_Subscriber_OnOneData Marker
// Subscriber Callback - GroundCommand_Subscriber
//# Start GroundCommand_Subscriber_OnOneData Marker
void TrajectoryPlanner::GroundCommand_Subscriber_OnOneData(const satellite_flight_application::GroundCommand::ConstPtr& received_data)
{
  // Business Logic for GroundCommand_Subscriber Subscriber
}
//# End GroundCommand_Subscriber_OnOneData Marker


// Destructor - Cleanup Ports & Timers
TrajectoryPlanner::~TrajectoryPlanner()
{
  TargetOrbit_Publisher.shutdown();
  SatState_Subscriber.shutdown();
  GroundCommand_Subscriber.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void TrajectoryPlanner::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Subscriber - SatState_Subscriber
  advertiseName = "SatState";
  if (portGroupMap.find("SatState_Subscriber") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["SatState_Subscriber"];
  ros::SubscribeOptions SatState_Subscriber_options;
  SatState_Subscriber_options = ros::SubscribeOptions::create<satellite_flight_application::SatState>
      (advertiseName.c_str(),
       1000,
       boost::bind(&TrajectoryPlanner::SatState_Subscriber_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->SatState_Subscriber = nh.subscribe(SatState_Subscriber_options);
  // Component Subscriber - GroundCommand_Subscriber
  advertiseName = "GroundCommand";
  if (portGroupMap.find("GroundCommand_Subscriber") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["GroundCommand_Subscriber"];
  ros::SubscribeOptions GroundCommand_Subscriber_options;
  GroundCommand_Subscriber_options = ros::SubscribeOptions::create<satellite_flight_application::GroundCommand>
      (advertiseName.c_str(),
       1000,
       boost::bind(&TrajectoryPlanner::GroundCommand_Subscriber_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->GroundCommand_Subscriber = nh.subscribe(GroundCommand_Subscriber_options);

  // Component Publisher - TargetOrbit_Publisher
  advertiseName = "TargetOrbit";
  if (portGroupMap.find("TargetOrbit_Publisher") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["TargetOrbit_Publisher"];
  this->TargetOrbit_Publisher = nh.advertise<cluster_flight_application::TargetOrbit>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&TrajectoryPlanner::Init, this, _1),
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
    return new TrajectoryPlanner(config,argc,argv);
  }
}
