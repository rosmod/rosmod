#include "client_server_package/Server.hpp"

//# Start User Globals Marker
#include <math.h>
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Server::Init(const ros::TimerEvent& event)
{
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Server Callback - server_port
//# Start PowerCallback Marker
bool Server::PowerCallback(client_server_package::Power::Request  &req,
  client_server_package::Power::Response &res)
{
  // Business Logic for server_port Server
  res.result = pow(req.base, req.exponent);
  return true;
}
//# End PowerCallback Marker


// Destructor - Cleanup Ports & Timers
Server::~Server()
{
  server_port.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Server::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Server - server_port
  advertiseName = "Power";
  if (portGroupMap.find("server_port") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["server_port"];
  ros::AdvertiseServiceOptions server_port_server_options;
  server_port_server_options = ros::AdvertiseServiceOptions::create<client_server_package::Power>
      (advertiseName.c_str(),
       boost::bind(&Server::PowerCallback, this, _1, _2),
       ros::VoidPtr(),
       &this->compQueue);
  this->server_port = nh.advertiseService(server_port_server_options);
 
  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Server::Init, this, _1),
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
    return new Server(config,argc,argv);
  }
}
