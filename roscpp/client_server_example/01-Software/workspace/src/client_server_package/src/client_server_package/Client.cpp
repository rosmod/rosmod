#include "client_server_package/Client.hpp"


//# Start User Globals Marker
#include <stdlib.h>
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Client::Init(const ros::TimerEvent& event)
{
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Timer Callback - client_timer
//# Start client_timerCallback Marker
void Client::client_timerCallback(const ros::TimerEvent& event)
{
  // Business Logic for client_timer Timer
  client_server_package::Power power_function;
  power_function.request.base = float(rand() % 10);
  power_function.request.exponent = float(rand() % 10);
  LOGGER.INFO("Client::Client Timer::Sending Request to calculate %f ^ %f", 
	      power_function.request.base, 
	      power_function.request.exponent);
  if (client_port.call(power_function))
    LOGGER.INFO("Client::Client Timer::Server Response - %f", power_function.response.result);
  else {
    LOGGER.ERROR("Client::Client Timer::Failed to invoke Server Operation");
    client_port.waitForExistence(ros::Duration(-1));
  }
}
//# End client_timerCallback Marker


// Destructor - Cleanup Ports & Timers
Client::~Client()
{
  client_timer.stop();
  client_port.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Client::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Scheduling Scheme is FIFO

  // Configure all required services associated with this component
  // Component Client - client_port
  advertiseName = "Power";
  if (portGroupMap.find("client_port") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["client_port"];
      this->client_port = nh.serviceClient<client_server_package::Power>(advertiseName.c_str()); 

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Client::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);
  this->initOneShotTimer.stop();
  // Component Timer - client_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(1.0),
     boost::bind(&Client::client_timerCallback, this, _1),
     &this->compQueue);
  this->client_timer = nh.createTimer(timer_options);
  this->client_timer.stop();

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


  this->comp_sync_pub = nh.advertise<std_msgs::Bool>("component_synchronization", 1000);
  
  ros::SubscribeOptions comp_sync_sub_options;
  comp_sync_sub_options = ros::SubscribeOptions::create<std_msgs::Bool>
    ("component_synchronization",
     1000,
     boost::bind(&Client::component_synchronization_OnOneData, this, _1),
     ros::VoidPtr(),
     &this->compQueue);
  this->comp_sync_sub = nh.subscribe(comp_sync_sub_options);

  ros::Time now = ros::Time::now();
  while ( this->comp_sync_sub.getNumPublishers() < this->num_comps_to_sync &&
	  (ros::Time::now() - now) < ros::Duration(comp_sync_timeout) );
  this->comp_sync_sub.shutdown();
  this->comp_sync_pub.shutdown();

  this->initOneShotTimer.start();
  this->client_timer.stop();
  
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new Client(config,argc,argv);
  }
}
