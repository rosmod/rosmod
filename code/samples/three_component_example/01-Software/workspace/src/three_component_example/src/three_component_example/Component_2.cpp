#include "three_component_example/Component_2.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Component_2::Init(const ros::TimerEvent& event)
{
  // Initialize Here
  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Server Callback - Service_Server
//# Start ComponentServiceCallback Marker
bool Component_2::ComponentServiceCallback(three_component_example::ComponentService::Request  &req,
  three_component_example::ComponentService::Response &res)
{
  // Business Logic for Service_Server Server
  three_component_example::ComponentName compName;
  compName.name = "Component_2";
  res.name = compName.name;
  LOGGER.INFO("Component_2::Name_Publisher::Publishing Component Name %s from ComponentServiceCallback", compName.name.c_str());
  Name_Publisher.publish(compName);
  return true;
}
//# End ComponentServiceCallback Marker

// Timer Callback - Timer_2
//# Start Timer_2Callback Marker
void Component_2::Timer_2Callback(const ros::TimerEvent& event)
{
  // Business Logic for Timer_2 Timer
  three_component_example::ComponentName compName;
  compName.name = "Component_2";
  LOGGER.INFO("Component_2::Name_Publisher::Publishing Component Name %s from Timer_2 Callback", compName.name.c_str());
  Name_Publisher.publish(compName);
}
//# End Timer_2Callback Marker


// Destructor - Cleanup Ports & Timers
Component_2::~Component_2()
{
  Timer_2.stop();
  Name_Publisher.shutdown();
  Service_Server.shutdown();
  //# Start Destructor Marker
                
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Component_2::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Publisher - Name_Publisher
  advertiseName = "ComponentName";
  if (portGroupMap.find("Name_Publisher") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["Name_Publisher"];
  this->Name_Publisher = nh.advertise<three_component_example::ComponentName>(advertiseName.c_str(), 1000);

  // Component Server - Service_Server
  advertiseName = "ComponentService";
  if (portGroupMap.find("Service_Server") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["Service_Server"];
  ros::AdvertiseServiceOptions Service_Server_server_options;
  Service_Server_server_options = ros::AdvertiseServiceOptions::create<three_component_example::ComponentService>
      (advertiseName.c_str(),
       boost::bind(&Component_2::ComponentServiceCallback, this, _1, _2),
       ros::VoidPtr(),
       &this->compQueue);
  this->Service_Server = nh.advertiseService(Service_Server_server_options);
 
  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Component_2::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - timer.properties["name"]
  timer_options = 
    ros::TimerOptions
    (ros::Duration(1.0),
     boost::bind(&Component_2::Timer_2Callback, this, _1),
     &this->compQueue);
  this->Timer_2 = nh.createTimer(timer_options);

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
    return new Component_2(config,argc,argv);
  }
}
