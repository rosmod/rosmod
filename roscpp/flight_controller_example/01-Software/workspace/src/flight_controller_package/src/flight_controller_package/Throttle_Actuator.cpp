#include "flight_controller_package/Throttle_Actuator.hpp"

KRPCI krpci_client;

//# Start User Globals Marker
uint64_t vesselID;
uint64_t partsID;
std::vector<uint64_t> engineIDs;
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Throttle_Actuator::Init(const ros::TimerEvent& event)
{
  // Initialize Here
  std::cout << "About to Connect" << std::endl;
  krpci_client.SetName(nodeName + "_" + compName);

  if (krpci_client.Connect()) {
    // Get Active Vessel
    krpci_client.get_ActiveVessel(vesselID);

    std::cout << "Vessel ID: " << vesselID << std::endl;

    // Get Reference Frame
    krpci_client.Vessel_get_Parts(vesselID, partsID);

    std::cout << "Parts ID: " << partsID << std::endl;

    // Get Flight ID
    krpci_client.Parts_get_Engines(partsID, engineIDs);
    
    for (int i=0; i < engineIDs.size(); i++)
      {
	std::cout << "Engine ID: " << engineIDs[i] << std::endl;
	if (i < engineIDs.size() / 2)
	  {
	    krpci_client.Engine_set_ThrustLimit(engineIDs[i],0);
	    std::cout << "Set Engine ID "<<engineIDs[i]<<" Thrust Limit to 0" << std::endl;
	  }
      }
  }

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Server Callback - throttle_control_server
//# Start Throttle_ControlCallback Marker
bool Throttle_Actuator::Throttle_ControlCallback(flight_controller_package::Throttle_Control::Request  &req,
  flight_controller_package::Throttle_Control::Response &res)
{
  // Business Logic for throttle_control_server Server

  return true;
}
//# End Throttle_ControlCallback Marker


// Destructor - Cleanup Ports & Timers
Throttle_Actuator::~Throttle_Actuator()
{
  throttle_control_server.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Throttle_Actuator::startUp()
{
  LOGGER.DEBUG("Entering Throttle_Actuator::startUp");
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Server - throttle_control_server
  advertiseName = "Throttle_Control";
  if (portGroupMap.find("throttle_control_server") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["throttle_control_server"];
  ros::AdvertiseServiceOptions throttle_control_server_server_options;
  throttle_control_server_server_options = ros::AdvertiseServiceOptions::create<flight_controller_package::Throttle_Control>
      (advertiseName.c_str(),
       boost::bind(&Throttle_Actuator::Throttle_ControlCallback, this, _1, _2),
       ros::VoidPtr(),
       &this->compQueue);
  this->throttle_control_server = nh.advertiseService(throttle_control_server_server_options);
 
  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Throttle_Actuator::Init, this, _1),
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
  LOGGER.DEBUG("Exiting Throttle_Actuator::startUp");
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new Throttle_Actuator(config,argc,argv);
  }
}
