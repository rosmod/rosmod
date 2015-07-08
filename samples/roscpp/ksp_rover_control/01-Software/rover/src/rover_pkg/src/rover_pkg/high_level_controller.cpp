#include "rover_pkg/high_level_controller.hpp"


//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void high_level_controller::Init(const ros::TimerEvent& event)
{
  LOGGER.DEBUG("Entering high_level_controller::Init");
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
  LOGGER.DEBUG("Exiting high_level_controller::Init");
}
//# End Init Marker

// Subscriber Callback - vessel_state_sub
//# Start vessel_state_sub_OnOneData Marker
void high_level_controller::vessel_state_sub_OnOneData(const rover_pkg::vessel_state::ConstPtr& received_data)
{
  LOGGER.DEBUG("Entering high_level_controller::vessel_state_sub_OnOneData");
  // Business Logic for vessel_state_sub Subscriber

  LOGGER.DEBUG("Exiting high_level_controller::vessel_state_sub_OnOneData");
}
//# End vessel_state_sub_OnOneData Marker

// Timer Callback - state_timer
//# Start state_timerCallback Marker
void high_level_controller::state_timerCallback(const ros::TimerEvent& event)
{
  LOGGER.DEBUG("Entering high_level_controller::state_timerCallback");
  // Business Logic for state_timer Timer

  LOGGER.DEBUG("Exiting high_level_controller::state_timerCallback");
}
//# End state_timerCallback Marker


// Destructor - Cleanup Ports & Timers
high_level_controller::~high_level_controller()
{
  state_timer.stop();
  goal_state_pub.shutdown();
  vessel_state_sub.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void high_level_controller::startUp()
{
  LOGGER.DEBUG("Entering high_level_controller::startUp");
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Subscriber - vessel_state_sub
  advertiseName = "vessel_state";
  if (portGroupMap.find("vessel_state_sub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["vessel_state_sub"];
  ros::SubscribeOptions vessel_state_sub_options;
  vessel_state_sub_options = ros::SubscribeOptions::create<rover_pkg::vessel_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&high_level_controller::vessel_state_sub_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->vessel_state_sub = nh.subscribe(vessel_state_sub_options);

  // Component Publisher - goal_state_pub
  advertiseName = "goal_state";
  if (portGroupMap.find("goal_state_pub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["goal_state_pub"];
  this->goal_state_pub = nh.advertise<rover_pkg::goal_state>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&high_level_controller::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - state_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.5),
     boost::bind(&high_level_controller::state_timerCallback, this, _1),
     &this->compQueue);
  this->state_timer = nh.createTimer(timer_options);

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

  LOGGER.DEBUG("Exiting high_level_controller::startUp");
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new high_level_controller(config,argc,argv);
  }
}
