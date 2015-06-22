#include "flight_controller_package/Heading_Sensor.hpp"

//# Start User Globals Marker
uint64_t vesselID;
uint64_t refFrame;
uint64_t flightID;
float heading;
//# End User Globals Marker

KRPCI krpci_client;

// Initialization Function
//# Start Init Marker
void Heading_Sensor::Init(const ros::TimerEvent& event)
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

// Timer Callback - heading_sensor_timer
//# Start heading_sensor_timerCallback Marker
void Heading_Sensor::heading_sensor_timerCallback(const ros::TimerEvent& event)
{
  // Business Logic for heading_sensor_timer Timer
  krpci_client.Flight_get_Heading(flightID, heading);

  flight_controller_package::Heading new_heading;
  new_heading.value = heading;
  heading_publisher.publish(new_heading);
  LOGGER.INFO("Heading_Publisher::New Heading Value: %f", new_heading.value);
}
//# End heading_sensor_timerCallback Marker


// Destructor - Cleanup Ports & Timers
Heading_Sensor::~Heading_Sensor()
{
  heading_sensor_timer.stop();
  heading_publisher.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Heading_Sensor::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Publisher - heading_publisher
  advertiseName = "Heading";
  if (portGroupMap.find("heading_publisher") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["heading_publisher"];
  this->heading_publisher = nh.advertise<flight_controller_package::Heading>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Heading_Sensor::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - heading_sensor_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.2),
     boost::bind(&Heading_Sensor::heading_sensor_timerCallback, this, _1),
     &this->compQueue);
  this->heading_sensor_timer = nh.createTimer(timer_options);

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
    return new Heading_Sensor(config,argc,argv);
  }
}
