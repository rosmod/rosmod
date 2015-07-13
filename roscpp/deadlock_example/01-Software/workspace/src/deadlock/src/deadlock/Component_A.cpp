#include "deadlock/Component_A.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Component_A::Init(const ros::TimerEvent& event)
{
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Server Callback - server_A
//# Start common_serviceCallback Marker
bool Component_A::common_serviceCallback(deadlock::common_service::Request  &req,
  deadlock::common_service::Response &res)
{
  // Business Logic for server_A Server
  res.return_value = true;
  return true;
}
//# End common_serviceCallback Marker

// Timer Callback - timer_A
//# Start timer_ACallback Marker
void Component_A::timer_ACallback(const ros::TimerEvent& event)
{
  // Business Logic for timer_A Timer
  deadlock::common_service srv;
  srv.request.name = "Component_A";
  LOGGER.INFO("Component_A::Timer::Sending Request to Component B");
  if (client_A.call(srv))
    LOGGER.INFO("Component_A::Timer::Server Response - %d", srv.response.return_value);
  else {
    LOGGER.ERROR("Component_A::Timer::Failed to invoke Server Operation");
    client_A.waitForExistence(ros::Duration(-1));
  }
}
//# End timer_ACallback Marker


// Destructor - Cleanup Ports & Timers
Component_A::~Component_A()
{
  timer_A.stop();
  server_A.shutdown();
  client_A.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Component_A::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Server - server_A
  advertiseName = "common_service";
  if (portGroupMap.find("server_A") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["server_A"];
  ros::AdvertiseServiceOptions server_A_server_options;
  server_A_server_options = ros::AdvertiseServiceOptions::create<deadlock::common_service>
      (advertiseName.c_str(),
       boost::bind(&Component_A::common_serviceCallback, this, _1, _2),
       ros::VoidPtr(),
       &this->compQueue);
  this->server_A = nh.advertiseService(server_A_server_options);
 
  // Configure all required services associated with this component
  // Component Client - client_A
  advertiseName = "common_service";
  if (portGroupMap.find("client_A") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["client_A"];
      this->client_A = nh.serviceClient<deadlock::common_service>(advertiseName.c_str()); 

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Component_A::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - timer_A
  timer_options = 
    ros::TimerOptions
    (ros::Duration(1.0),
     boost::bind(&Component_A::timer_ACallback, this, _1),
     &this->compQueue);
  this->timer_A = nh.createTimer(timer_options);

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
    return new Component_A(config,argc,argv);
  }
}
