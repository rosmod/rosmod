#include "ksp_stearwing_controller/Sensor_Component.hpp"

//# Start User Globals Marker
uint64_t vesselID;
uint64_t surfaceRefFrameID;
uint64_t flightID;
uint64_t controlID;
uint64_t orbitalRefFrameID;
uint64_t orbitalFlightID;
float throttle;
float pitch;
float roll;
float heading;
double altitude;
double latitude;
double longitude;
double speed;
//# End User Globals Marker

KRPCI krpci_client;

// Initialization Function
//# Start Init Marker
void Sensor_Component::Init(const ros::TimerEvent& event)
{
  // Initialize Here
  if (krpci_client.Connect()) {
    krpci_client.get_ActiveVessel(vesselID);
    krpci_client.Vessel_get_SurfaceReferenceFrame(vesselID, surfaceRefFrameID);
    krpci_client.Vessel_get_OrbitalReferenceFrame(vesselID, orbitalRefFrameID);
    krpci_client.Vessel_Flight(vesselID, surfaceRefFrameID, flightID);
    krpci_client.Vessel_Flight(vesselID, orbitalRefFrameID, orbitalFlightID);
    krpci_client.Vessel_get_Control(vesselID, controlID);
  }

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Timer Callback - sensor_timer
//# Start sensor_timerCallback Marker
void Sensor_Component::sensor_timerCallback(const ros::TimerEvent& event)
{
  // Business Logic for sensor_timer Timer
  krpci_client.Control_get_Throttle(controlID, throttle);
  krpci_client.Flight_get_Pitch(flightID, pitch);
  krpci_client.Flight_get_Roll(flightID, roll);
  krpci_client.Flight_get_Heading(flightID, heading);
  krpci_client.Flight_get_MeanAltitude(flightID, altitude);
  krpci_client.Flight_get_Latitude(flightID, latitude);
  krpci_client.Flight_get_Longitude(flightID, longitude);
  krpci_client.Flight_get_Speed(flightID, speed);

  double trans_x, trans_y, trans_z;
  krpci_client.Flight_get_Velocity(orbitalFlightID, trans_x, trans_y, trans_z);
  //LOGGER.INFO("ORBITAL FLIGHT VELOCITY:: %f, %f, %f",trans_x,trans_y,trans_z);
  double vel_x, vel_y, vel_z;
  krpci_client.TransformDirection(trans_x, trans_y, trans_z, orbitalRefFrameID, surfaceRefFrameID, vel_x, vel_y, vel_z);
  //LOGGER.INFO("VELOCITY:: %f, %f, %f",vel_x,vel_y,vel_z);
  double vel_speed = sqrt(vel_x * vel_x + vel_y * vel_y + vel_z * vel_z);
  //LOGGER.INFO("SPEED:: %f",vel_speed);

  ksp_stearwing_controller::Sensor_Reading new_reading;
  new_reading.throttle = throttle;
  new_reading.pitch = pitch;
  new_reading.roll = roll;
  new_reading.heading = heading;
  new_reading.altitude = altitude;
  new_reading.latitude = latitude;
  new_reading.longitude = longitude;
  new_reading.speed = speed;
  sensor_publisher.publish(new_reading);
  LOGGER.INFO("Sensor Publisher::Throttle=%f; Pitch=%f; Roll=%f; Heading=%f, Altitude=%f; Latitude=%f; Longitude=%f; Speed=%f", throttle, pitch, roll, heading, altitude, latitude, longitude, speed);
}
//# End sensor_timerCallback Marker


// Destructor - Cleanup Ports & Timers
Sensor_Component::~Sensor_Component()
{
  sensor_timer.stop();
  sensor_publisher.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Sensor_Component::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Publisher - sensor_publisher
  advertiseName = "Sensor_Reading";
  if (portGroupMap.find("sensor_publisher") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["sensor_publisher"];
  this->sensor_publisher = nh.advertise<ksp_stearwing_controller::Sensor_Reading>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Sensor_Component::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - sensor_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.1),
     boost::bind(&Sensor_Component::sensor_timerCallback, this, _1),
     &this->compQueue);
  this->sensor_timer = nh.createTimer(timer_options);

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
    return new Sensor_Component(config,argc,argv);
  }
}
