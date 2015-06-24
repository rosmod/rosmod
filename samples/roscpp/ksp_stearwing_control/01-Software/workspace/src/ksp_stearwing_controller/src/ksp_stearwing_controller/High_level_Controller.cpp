#include "ksp_stearwing_controller/High_level_Controller.hpp"

//# Start User Globals Marker
uint64_t vesselID;
uint64_t controlID;
//# End User Globals Marker

KRPCI krpci_client;

// Initialization Function
//# Start Init Marker
void High_level_Controller::Init(const ros::TimerEvent& event)
{
  // Initialize Here
  if (krpci_client.Connect()) {
    krpci_client.get_ActiveVessel(vesselID);
    krpci_client.Vessel_get_Control(vesselID, controlID);
  }
  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Subscriber Callback - sensor_subscriber
//# Start sensor_subscriber_OnOneData Marker
void High_level_Controller::sensor_subscriber_OnOneData(const ksp_stearwing_controller::Sensor_Reading::ConstPtr& received_data)
{
  // Business Logic for sensor_subscriber Subscriber
  current_throttle = received_data->throttle;
  current_pitch = received_data->pitch;
  current_roll = received_data->roll;
  current_heading = received_data->heading;
  current_altitude = received_data->altitude;
  current_latitude = received_data->latitude;
  current_longitude = received_data->longitude;
  current_speed = received_data->speed;
  LOGGER.INFO("Sensor Subscriber::Throttle=%f; Pitch=%f; Roll=%f; Heading=%f, Altitude=%f; Latitude=%f; Longitude=%f; Speed=%f", current_throttle, current_pitch, current_roll, current_heading, current_altitude, current_latitude, current_longitude, current_speed);
}
//# End sensor_subscriber_OnOneData Marker

// Timer Callback - flight_control_timer
//# Start flight_control_timerCallback Marker
void High_level_Controller::flight_control_timerCallback(const ros::TimerEvent& event)
{
  // Business Logic for flight_control_timer Timer
}
//# End flight_control_timerCallback Marker


// Destructor - Cleanup Ports & Timers
High_level_Controller::~High_level_Controller()
{
  flight_control_timer.stop();
  pid_control_publisher.shutdown();
  sensor_subscriber.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void High_level_Controller::startUp()
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
       boost::bind(&High_level_Controller::sensor_subscriber_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->sensor_subscriber = nh.subscribe(sensor_subscriber_options);

  // Component Publisher - pid_control_publisher
  advertiseName = "Control_Command";
  if (portGroupMap.find("pid_control_publisher") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["pid_control_publisher"];
  this->pid_control_publisher = nh.advertise<ksp_stearwing_controller::Control_Command>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&High_level_Controller::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - flight_control_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.5),
     boost::bind(&High_level_Controller::flight_control_timerCallback, this, _1),
     &this->compQueue);
  this->flight_control_timer = nh.createTimer(timer_options);

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

  krpci_client.SetName(nodeName + "_" + compName);
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new High_level_Controller(config,argc,argv);
  }
}
