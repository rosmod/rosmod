#include "flight_controller_package/High_level_Controller.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void High_level_Controller::Init(const ros::TimerEvent& event)
{
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Subscriber Callback - pitch_subscriber
//# Start pitch_subscriber_OnOneData Marker
void High_level_Controller::pitch_subscriber_OnOneData(const flight_controller_package::Pitch::ConstPtr& received_data)
{
  // Business Logic for pitch_subscriber Subscriber
}
//# End pitch_subscriber_OnOneData Marker
// Subscriber Callback - roll_subscriber
//# Start roll_subscriber_OnOneData Marker
void High_level_Controller::roll_subscriber_OnOneData(const flight_controller_package::Roll::ConstPtr& received_data)
{
  // Business Logic for roll_subscriber Subscriber
}
//# End roll_subscriber_OnOneData Marker
// Subscriber Callback - heading_subscriber
//# Start heading_subscriber_OnOneData Marker
void High_level_Controller::heading_subscriber_OnOneData(const flight_controller_package::Heading::ConstPtr& received_data)
{
  // Business Logic for heading_subscriber Subscriber
}
//# End heading_subscriber_OnOneData Marker
// Subscriber Callback - altitude_subscriber
//# Start altitude_subscriber_OnOneData Marker
void High_level_Controller::altitude_subscriber_OnOneData(const flight_controller_package::Altitude::ConstPtr& received_data)
{
  // Business Logic for altitude_subscriber Subscriber
}
//# End altitude_subscriber_OnOneData Marker

// Timer Callback - high_level_timer
//# Start high_level_timerCallback Marker
void High_level_Controller::high_level_timerCallback(const ros::TimerEvent& event)
{
  // Business Logic for high_level_timer Timer
}
//# End high_level_timerCallback Marker


// Destructor - Cleanup Ports & Timers
High_level_Controller::~High_level_Controller()
{
  high_level_timer.stop();
  pitch_subscriber.shutdown();
  roll_subscriber.shutdown();
  heading_subscriber.shutdown();
  altitude_subscriber.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void High_level_Controller::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Subscriber - pitch_subscriber
  advertiseName = "Pitch";
  if (portGroupMap.find("pitch_subscriber") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["pitch_subscriber"];
  ros::SubscribeOptions pitch_subscriber_options;
  pitch_subscriber_options = ros::SubscribeOptions::create<flight_controller_package::Pitch>
      (advertiseName.c_str(),
       1000,
       boost::bind(&High_level_Controller::pitch_subscriber_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->pitch_subscriber = nh.subscribe(pitch_subscriber_options);
  // Component Subscriber - roll_subscriber
  advertiseName = "Roll";
  if (portGroupMap.find("roll_subscriber") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["roll_subscriber"];
  ros::SubscribeOptions roll_subscriber_options;
  roll_subscriber_options = ros::SubscribeOptions::create<flight_controller_package::Roll>
      (advertiseName.c_str(),
       1000,
       boost::bind(&High_level_Controller::roll_subscriber_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->roll_subscriber = nh.subscribe(roll_subscriber_options);
  // Component Subscriber - heading_subscriber
  advertiseName = "Heading";
  if (portGroupMap.find("heading_subscriber") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["heading_subscriber"];
  ros::SubscribeOptions heading_subscriber_options;
  heading_subscriber_options = ros::SubscribeOptions::create<flight_controller_package::Heading>
      (advertiseName.c_str(),
       1000,
       boost::bind(&High_level_Controller::heading_subscriber_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->heading_subscriber = nh.subscribe(heading_subscriber_options);
  // Component Subscriber - altitude_subscriber
  advertiseName = "Altitude";
  if (portGroupMap.find("altitude_subscriber") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["altitude_subscriber"];
  ros::SubscribeOptions altitude_subscriber_options;
  altitude_subscriber_options = ros::SubscribeOptions::create<flight_controller_package::Altitude>
      (advertiseName.c_str(),
       1000,
       boost::bind(&High_level_Controller::altitude_subscriber_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->altitude_subscriber = nh.subscribe(altitude_subscriber_options);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&High_level_Controller::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - high_level_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.5),
     boost::bind(&High_level_Controller::high_level_timerCallback, this, _1),
     &this->compQueue);
  this->high_level_timer = nh.createTimer(timer_options);

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
    return new High_level_Controller(config,argc,argv);
  }
}
