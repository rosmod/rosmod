#include "pub_sub_tg/sender.hpp"


//# Start User Globals Marker

void sender::message_pub_wrapper(const pub_sub_tg::message& msg)
{
  // CHECK AGAINST PRIVATE VARIABLE : DEACTIVATED
  // CHECK AGAINST PROFILE (INCLUDING METERING FROM RECEIVER)
  // IF EVERYTHING IS ALRIGHT, PASS IT THROUGH:
  message_pub.publish(msg);
}

//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void sender::Init(const ros::TimerEvent& event)
{
  LOGGER.DEBUG("Entering sender::Init");
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
  LOGGER.DEBUG("Exiting sender::Init");
}
//# End Init Marker

// Server Callback - oob_server
//# Start oob_commCallback Marker
bool sender::oob_commCallback(pub_sub_tg::oob_comm::Request  &req,
  pub_sub_tg::oob_comm::Response &res)
{
  LOGGER.DEBUG("Entering sender::oob_commCallback");
  // Business Logic for oob_server Server

  LOGGER.DEBUG("Exiting sender::oob_commCallback");
  return true;
}
//# End oob_commCallback Marker


// Destructor - Cleanup Ports & Timers
sender::~sender()
{
  message_pub.shutdown();
  oob_server.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void sender::startUp()
{
  LOGGER.DEBUG("Entering sender::startUp");
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Publisher - message_pub
  advertiseName = "message";
  if (portGroupMap.find("message_pub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["message_pub"];
  this->message_pub = nh.advertise<pub_sub_tg::message>(advertiseName.c_str(), 1000);

  // Component Server - oob_server
  advertiseName = "oob_comm";
  if (portGroupMap.find("oob_server") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["oob_server"];
  ros::AdvertiseServiceOptions oob_server_server_options;
  oob_server_server_options = ros::AdvertiseServiceOptions::create<pub_sub_tg::oob_comm>
      (advertiseName.c_str(),
       boost::bind(&sender::oob_commCallback, this, _1, _2),
       ros::VoidPtr(),
       &this->compQueue);
  this->oob_server = nh.advertiseService(oob_server_server_options);
 
  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&sender::Init, this, _1),
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

  LOGGER.DEBUG("Exiting sender::startUp");
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new sender(config,argc,argv);
  }
}
