#include "satellite_flight_application/OrbitController.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void OrbitController::Init(const ros::TimerEvent& event)
{
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Subscriber Callback - TargetOrbit_Subscriber
//# Start TargetOrbit_Subscriber_OnOneData Marker
void OrbitController::TargetOrbit_Subscriber_OnOneData(const cluster_flight_application::TargetOrbit::ConstPtr& received_data)
{
  // Business Logic for TargetOrbit_Subscriber Subscriber
}
//# End TargetOrbit_Subscriber_OnOneData Marker

// Timer Callback - OrbitController_Timer
//# Start OrbitController_TimerCallback Marker
void OrbitController::OrbitController_TimerCallback(const ros::TimerEvent& event)
{
  // Business Logic for OrbitController_Timer Timer
}
//# End OrbitController_TimerCallback Marker


// Destructor - Cleanup Ports & Timers
OrbitController::~OrbitController()
{
  OrbitController_Timer.stop();
  SatState_Publisher.shutdown();
  TargetOrbit_Subscriber.shutdown();
  SatelliteState_Client.shutdown();
  ThrusterComm_Client.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void OrbitController::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Subscriber - TargetOrbit_Subscriber
  advertiseName = "TargetOrbit";
  if (portGroupMap.find("TargetOrbit_Subscriber") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["TargetOrbit_Subscriber"];
  ros::SubscribeOptions TargetOrbit_Subscriber_options;
  TargetOrbit_Subscriber_options = ros::SubscribeOptions::create<cluster_flight_application::TargetOrbit>
      (advertiseName.c_str(),
       1000,
       boost::bind(&OrbitController::TargetOrbit_Subscriber_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->TargetOrbit_Subscriber = nh.subscribe(TargetOrbit_Subscriber_options);

  // Component Publisher - SatState_Publisher
  advertiseName = "SatState";
  if (portGroupMap.find("SatState_Publisher") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["SatState_Publisher"];
  this->SatState_Publisher = nh.advertise<satellite_flight_application::SatState>(advertiseName.c_str(), 1000);

  // Configure all required services associated with this component
  // Component Client - SatelliteState_Client
  advertiseName = "SatelliteState";
  if (portGroupMap.find("SatelliteState_Client") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["SatelliteState_Client"];
      this->SatelliteState_Client = nh.serviceClient<satellite_flight_application::SatelliteState>(advertiseName.c_str()); 
  // Component Client - ThrusterComm_Client
  advertiseName = "ThrusterComm";
  if (portGroupMap.find("ThrusterComm_Client") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["ThrusterComm_Client"];
      this->ThrusterComm_Client = nh.serviceClient<satellite_flight_application::ThrusterComm>(advertiseName.c_str()); 

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&OrbitController::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - timer.properties["name"]
  timer_options = 
    ros::TimerOptions
    (ros::Duration(1.0),
     boost::bind(&OrbitController::OrbitController_TimerCallback, this, _1),
     &this->compQueue);
  this->OrbitController_Timer = nh.createTimer(timer_options);

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
    return new OrbitController(config,argc,argv);
  }
}
