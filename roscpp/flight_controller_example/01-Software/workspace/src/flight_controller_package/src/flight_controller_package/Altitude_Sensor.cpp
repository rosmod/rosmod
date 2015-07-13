#include "flight_controller_package/Altitude_Sensor.hpp"

KRPCI krpci_client;

//# Start User Globals Marker
uint64_t vesselID;
uint64_t refFrame;
uint64_t flightID;
double altitude;
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Altitude_Sensor::Init(const ros::TimerEvent& event)
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

// Timer Callback - altitude_sensor_timer
//# Start altitude_sensor_timerCallback Marker
void Altitude_Sensor::altitude_sensor_timerCallback(const ros::TimerEvent& event)
{
  // Business Logic for altitude_sensor_timer Timer
  krpci_client.Flight_get_MeanAltitude(flightID, altitude);

  flight_controller_package::Altitude new_altitude;
  new_altitude.value = altitude;
  altitude_publisher.publish(new_altitude);
  LOGGER.INFO("Altitude_Publisher::New Altitude Value: %f", new_altitude.value);
}
//# End altitude_sensor_timerCallback Marker


// Destructor - Cleanup Ports & Timers
Altitude_Sensor::~Altitude_Sensor()
{
  altitude_sensor_timer.stop();
  altitude_publisher.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Altitude_Sensor::startUp()
{
  LOGGER.DEBUG("Entering Altitude_Sensor::startUp");
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Publisher - altitude_publisher
  advertiseName = "Altitude";
  if (portGroupMap.find("altitude_publisher") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["altitude_publisher"];
  this->altitude_publisher = nh.advertise<flight_controller_package::Altitude>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Altitude_Sensor::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - altitude_sensor_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.2),
     boost::bind(&Altitude_Sensor::altitude_sensor_timerCallback, this, _1),
     &this->compQueue);
  this->altitude_sensor_timer = nh.createTimer(timer_options);

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
  LOGGER.DEBUG("Exiting Altitude_Sensor::startUp");
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new Altitude_Sensor(config,argc,argv);
  }
}
