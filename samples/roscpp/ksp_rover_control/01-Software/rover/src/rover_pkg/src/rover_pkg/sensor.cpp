#include "rover_pkg/sensor.hpp"

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
float pitch;
float roll;
float heading;
double latitude;
double longitude;
double speed;
bool landing_gear;
bool brakes;
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void sensor::Init(const ros::TimerEvent& event)
{
  LOGGER.DEBUG("Entering sensor::Init");
  // Initialize Here
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
  LOGGER.DEBUG("Exiting sensor::Init");
}
//# End Init Marker

// Timer Callback - sensor_timer
//# Start sensor_timerCallback Marker
void sensor::sensor_timerCallback(const ros::TimerEvent& event)
{
  LOGGER.DEBUG("Entering sensor::sensor_timerCallback");
  // Business Logic for sensor_timer Timer
  krpci_client.Flight_get_Pitch(flightID, pitch);
  krpci_client.Flight_get_Roll(flightID, roll);
  krpci_client.Flight_get_Heading(flightID, heading);
  krpci_client.Flight_get_Latitude(flightID, latitude);
  krpci_client.Flight_get_Longitude(flightID, longitude);
  krpci_client.Control_get_Gear(controlID, landing_gear);
  krpci_client.Control_get_Brakes(controlID, brakes);

  double vel[3];
  krpci_client.Vessel_Velocity(vesselID, orbitalRefFrameID, vel[0], vel[1], vel[2]);
  speed = sqrt(vel[0] * vel[0] + vel[1] * vel[1] + vel[2] * vel[2]);

  rover_pkg::vessel_state state;
  state.pitch = pitch;
  state.roll = roll;
  state.heading = heading;
  state.latitude = latitude;
  state.longitude = longitude;
  state.speed = speed;
  state.landing_gear = landing_gear;
  state.brakes = brakes;
  sensor_publisher.publish(state);
  LOGGER.DEBUG("Exiting sensor::sensor_timerCallback");
}
//# End sensor_timerCallback Marker


// Destructor - Cleanup Ports & Timers
sensor::~sensor()
{
  sensor_timer.stop();
  vessel_state_pub.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void sensor::startUp()
{
  LOGGER.DEBUG("Entering sensor::startUp");
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Publisher - vessel_state_pub
  advertiseName = "vessel_state";
  if (portGroupMap.find("vessel_state_pub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["vessel_state_pub"];
  this->vessel_state_pub = nh.advertise<rover_pkg::vessel_state>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&sensor::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - sensor_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.1),
     boost::bind(&sensor::sensor_timerCallback, this, _1),
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
  LOGGER.DEBUG("Exiting sensor::startUp");
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new sensor(config,argc,argv);
  }
}
