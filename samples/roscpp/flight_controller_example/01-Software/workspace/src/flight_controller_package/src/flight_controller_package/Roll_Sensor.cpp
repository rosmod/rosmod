#include "flight_controller_package/Roll_Sensor.hpp"

KRPCI krpci_client;

//# Start User Globals Marker
uint64_t vesselID;
uint64_t refFrame;
uint64_t flightID;
float roll;
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Roll_Sensor::Init(const ros::TimerEvent& event)
{
  // Initialize Here
  if (!krpci_client.Connect())
    LOGGER.INFO("Couldn't connect to KRPC Server!");

  // Get Active Vessel
  krpci_client.get_ActiveVessel(vesselID);

  // Get Reference Frame
  krpci_client.Vessel_get_SurfaceReferenceFrame(vesselID, refFrame);

  // Get Flight ID
  krpci_client.Vessel_Flight(vesselID, refFrame, flightID);

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Timer Callback - roll_sensor_timer
//# Start roll_sensor_timerCallback Marker
void Roll_Sensor::roll_sensor_timerCallback(const ros::TimerEvent& event)
{
  // Business Logic for roll_sensor_timer Timer
  krpci_client.Flight_get_Roll(flightID, roll);

  flight_controller_package::Roll new_roll;
  new_roll.value = roll;
  roll_publisher.publish(new_roll);
  LOGGER.INFO("Roll_Publisher::New Roll Value: %f", new_roll.value);
}
//# End roll_sensor_timerCallback Marker


// Destructor - Cleanup Ports & Timers
Roll_Sensor::~Roll_Sensor()
{
  roll_sensor_timer.stop();
  roll_publisher.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Roll_Sensor::startUp()
{
  LOGGER.DEBUG("Entering Roll_Sensor::startUp");
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Publisher - roll_publisher
  advertiseName = "Roll";
  if (portGroupMap.find("roll_publisher") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["roll_publisher"];
  this->roll_publisher = nh.advertise<flight_controller_package::Roll>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Roll_Sensor::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - roll_sensor_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.2),
     boost::bind(&Roll_Sensor::roll_sensor_timerCallback, this, _1),
     &this->compQueue);
  this->roll_sensor_timer = nh.createTimer(timer_options);

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
  LOGGER.DEBUG("Exiting Roll_Sensor::startUp");
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new Roll_Sensor(config,argc,argv);
  }
}
