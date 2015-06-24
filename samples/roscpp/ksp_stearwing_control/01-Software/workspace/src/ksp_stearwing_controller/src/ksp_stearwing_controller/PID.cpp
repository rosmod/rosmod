#include "ksp_stearwing_controller/PID.hpp"

//# Start User Globals Marker
//# End User Globals Marker


// Initialization Function
//# Start Init Marker
void PID::Init(const ros::TimerEvent& event)
{
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Subscriber Callback - sensor_subscriber
//# Start sensor_subscriber_OnOneData Marker
void PID::sensor_subscriber_OnOneData(const ksp_stearwing_controller::Sensor_Reading::ConstPtr& received_data)
{
  // Business Logic for sensor_subscriber Subscriber
  current_throttle = received_data->throttle;
  current_pitch = received_data->pitch;
  current_roll = received_data->roll;
  current_heading = received_data->heading;
  current_altitude = received_data->altitude;
  current_speed = received_data->speed;
  LOGGER.INFO("Sensor Subscriber::Throttle=%f; Pitch=%f; Roll=%f; Heading=%f, Altitude=%f; Speed=%f", current_throttle, current_pitch, current_roll, current_heading, current_altitude, current_speed);
}
//# End sensor_subscriber_OnOneData Marker
// Subscriber Callback - pid_control_subscriber
//# Start pid_control_subscriber_OnOneData Marker
void PID::pid_control_subscriber_OnOneData(const ksp_stearwing_controller::Control_Command::ConstPtr& received_data)
{
  // Business Logic for pid_control_subscriber Subscriber
  goal_heading = received_data->goal_heading;
  goal_altitude = received_data->goal_altitude;
  goal_speed = received_data->goal_speed;
  LOGGER.INFO("Control Subscriber::Heading=%f, Altitude=%f; Speed=%f", current_heading, current_altitude, current_speed);
}
//# End pid_control_subscriber_OnOneData Marker

// Timer Callback - pid_timer
//# Start pid_timerCallback Marker
void PID::pid_timerCallback(const ros::TimerEvent& event)
{
  // Business Logic for pid_timer Timer
  float new_pitch = 0;
  float new_roll = 0;
  float new_yaw = 0;
  float new_throttle = 0;

  // NEED TO DO PID HERE

  ksp_stearwing_controller::Actuation_Command new_actuation;
  new_actuation.new_pitch = new_pitch;
  new_actuation.new_roll = new_roll;
  new_actuation.new_yaw = new_yaw;
  new_actuation.new_throttle = new_throttle;
  actuator_control_publisher.publish(new_actuation);
}
//# End pid_timerCallback Marker


// Destructor - Cleanup Ports & Timers
PID::~PID()
{
  pid_timer.stop();
  actuator_control_publisher.shutdown();
  sensor_subscriber.shutdown();
  pid_control_subscriber.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void PID::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Subscriber - sensor_subscriber
  advertiseName = "Sensor_Reading";
  if (portGroupMap.find("sensor_subscriber") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["sensor_subscriber"];
  ros::SubscribeOptions sensor_subscriber_options;
  sensor_subscriber_options = ros::SubscribeOptions::create<ksp_stearwing_controller::Sensor_Reading>
      (advertiseName.c_str(),
       1000,
       boost::bind(&PID::sensor_subscriber_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->sensor_subscriber = nh.subscribe(sensor_subscriber_options);
  // Component Subscriber - pid_control_subscriber
  advertiseName = "Control_Command";
  if (portGroupMap.find("pid_control_subscriber") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["pid_control_subscriber"];
  ros::SubscribeOptions pid_control_subscriber_options;
  pid_control_subscriber_options = ros::SubscribeOptions::create<ksp_stearwing_controller::Control_Command>
      (advertiseName.c_str(),
       1000,
       boost::bind(&PID::pid_control_subscriber_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->pid_control_subscriber = nh.subscribe(pid_control_subscriber_options);

  // Component Publisher - actuator_control_publisher
  advertiseName = "Actuation_Command";
  if (portGroupMap.find("actuator_control_publisher") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["actuator_control_publisher"];
  this->actuator_control_publisher = nh.advertise<ksp_stearwing_controller::Actuation_Command>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&PID::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - pid_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.2),
     boost::bind(&PID::pid_timerCallback, this, _1),
     &this->compQueue);
  this->pid_timer = nh.createTimer(timer_options);

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
    return new PID(config,argc,argv);
  }
}
