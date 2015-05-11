#include "three_component_example/Component_3.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Component_3::Init(const ros::TimerEvent& event)
{
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Timer Callback - Timer_3
//# Start Timer_3Callback Marker
void Component_3::Timer_3Callback(const ros::TimerEvent& event)
{
  // Business Logic for Timer_3 Timer
  three_component_example::ComponentService srv;
  srv.request.operation = "GetComponentName";
  LOGGER.INFO("Component_3::Timer_3::Invoking ComponentService from Timer_3 Callback");
  if (Service_Client.call(srv))
    LOGGER.INFO("Component_3::Timer_3::Got Name of Server Component: %s", srv.response.name.c_str());
  else {
    LOGGER.ERROR("Component_3::Timer_3::Failed to Invoke Server Operation");
    Service_Client.waitForExistence(ros::Duration(-1));
  }
}
//# End Timer_3Callback Marker


// Destructor - Cleanup Ports & Timers
Component_3::~Component_3()
{
  Timer_3.stop();
  Service_Client.shutdown();
  //# Start Destructor Marker
            
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Component_3::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Configure all required services associated with this component
  // Component Client - Service_Client
  advertiseName = "ComponentService";
  if (portGroupMap.find("Service_Client") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["Service_Client"];
      this->Service_Client = nh.serviceClient<three_component_example::ComponentService>(advertiseName.c_str()); 

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Component_3::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - timer.properties["name"]
  timer_options = 
    ros::TimerOptions
    (ros::Duration(2.0),
     boost::bind(&Component_3::Timer_3Callback, this, _1),
     &this->compQueue);
  this->Timer_3 = nh.createTimer(timer_options);

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
  // LOGGER.SET_LOG_LEVELS(logLevels);
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new Component_3(config,argc,argv);
  }
}
