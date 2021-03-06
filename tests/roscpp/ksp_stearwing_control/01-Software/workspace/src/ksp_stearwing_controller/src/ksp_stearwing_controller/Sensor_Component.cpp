#include "ksp_stearwing_controller/Sensor_Component.hpp"

KRPCI krpci_client;

//# Start User Globals Marker
uint64_t vesselID;
uint64_t orbitID;
uint64_t bodyID;
uint64_t surfaceRefFrameID;
uint64_t flightID;
uint64_t controlID;
uint64_t orbitalRefFrameID;
uint64_t orbitalFlightID;
float throttle;
float pitch;
float roll;
float heading;
double mean_altitude;
double latitude;
double longitude;
double speed;
double surface_altitude;
bool landing_gear;
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Sensor_Component::Init(const ros::TimerEvent& event)
{

  krpci_client.SetIP("191.168.127.100");
  for (int i=0;i<node_argc;i++)
    {
      if (!strcmp(node_argv[i],"--krpc_ip"))
	{
	  krpci_client.SetIP(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--krpc_port"))
	{
	  krpci_client.SetPort(atoi(node_argv[i+1]));	  
	}      
    }

  // Initialize Here
  if (krpci_client.Connect()) {
    krpci_client.get_ActiveVessel(vesselID);
    krpci_client.Vessel_get_SurfaceReferenceFrame(vesselID, surfaceRefFrameID);
    krpci_client.Vessel_get_Orbit(vesselID, orbitID);
    krpci_client.Orbit_get_Body(orbitID, bodyID);
    krpci_client.CelestialBody_get_ReferenceFrame(bodyID, orbitalRefFrameID);
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
  krpci_client.Flight_get_MeanAltitude(flightID, mean_altitude);
  krpci_client.Flight_get_Latitude(flightID, latitude);
  krpci_client.Flight_get_Longitude(flightID, longitude);
  //krpci_client.Flight_get_Speed(flightID, speed);
  krpci_client.Flight_get_SurfaceAltitude(flightID, surface_altitude);
  krpci_client.Control_get_Gear(controlID, landing_gear);

  double vel[3];
  krpci_client.Vessel_Velocity(vesselID, orbitalRefFrameID, vel[0], vel[1], vel[2]);
  speed = sqrt(vel[0] * vel[0] + vel[1] * vel[1] + vel[2] * vel[2]);

  ksp_stearwing_controller::Sensor_Reading new_reading;
  new_reading.throttle = throttle;
  new_reading.pitch = pitch;
  new_reading.roll = roll;
  new_reading.heading = heading;
  new_reading.mean_altitude = mean_altitude;
  new_reading.latitude = latitude;
  new_reading.longitude = longitude;
  new_reading.speed = speed;
  new_reading.surface_altitude = surface_altitude;
  new_reading.landing_gear = landing_gear;
  sensor_publisher.publish(new_reading);
  LOGGER.INFO("Sensor Publisher::Throttle=%f; Pitch=%f; Roll=%f; Heading=%f, Mean_Altitude=%f; Latitude=%f; Longitude=%f; Speed=%f; Landing_Gear=%d", throttle, pitch, roll, heading, mean_altitude, latitude, longitude, speed, landing_gear);
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
  LOGGER.DEBUG("Entering Sensor_Component::startUp");
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
  LOGGER.DEBUG("Exiting Sensor_Component::startUp");
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new Sensor_Component(config,argc,argv);
  }
}
