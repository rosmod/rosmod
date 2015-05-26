#include "deadlock/Component_B.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Component_B::Init(const ros::TimerEvent& event)
{
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Server Callback - server_B
//# Start common_serviceCallback Marker
bool Component_B::common_serviceCallback(deadlock::common_service::Request  &req,
  deadlock::common_service::Response &res)
{
  // Business Logic for server_B Server
  deadlock::common_service srv;
  LOGGER.INFO("Component_B::Server::Sending Request to Component A Server");
  if (client_B.call(srv))
    LOGGER.INFO("Component_B::Server::Server Response - %d", srv.response.return_value);
  else {
    LOGGER.ERROR("Component_B::Server::Failed to invoke Component A Server Operation");
    client_B.waitForExistence(ros::Duration(-1));
  }
  res.return_value = true;
  return true;
}
//# End common_serviceCallback Marker


// Destructor - Cleanup Ports & Timers
Component_B::~Component_B()
{
  server_B.shutdown();
  client_B.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Component_B::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Server - server_B
  advertiseName = "common_service";
  if (portGroupMap.find("server_B") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["server_B"];
  ros::AdvertiseServiceOptions server_B_server_options;
  server_B_server_options = ros::AdvertiseServiceOptions::create<deadlock::common_service>
      (advertiseName.c_str(),
       boost::bind(&Component_B::common_serviceCallback, this, _1, _2),
       ros::VoidPtr(),
       &this->compQueue);
  this->server_B = nh.advertiseService(server_B_server_options);
 
  // Configure all required services associated with this component
  // Component Client - client_B
  advertiseName = "common_service";
  if (portGroupMap.find("client_B") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["client_B"];
      this->client_B = nh.serviceClient<deadlock::common_service>(advertiseName.c_str()); 

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Component_B::Init, this, _1),
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
    return new Component_B(config,argc,argv);
  }
}
