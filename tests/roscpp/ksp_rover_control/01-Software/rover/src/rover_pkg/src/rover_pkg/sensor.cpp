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
int wp_count=1;
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void sensor::Init(const ros::TimerEvent& event)
{
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
}
//# End Init Marker


// Timer Callback - sensor_timer
//# Start sensor_timerCallback Marker
void sensor::sensor_timerCallback(const ros::TimerEvent& event)
{
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
  vessel_state_pub.publish(state);
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
  ros::NodeHandle nh;
  std::string advertiseName;

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

  // Prepare logging periodicity
  LOGGER.SET_PERIODICITY(is_periodic_logging);
  LOGGER.CHANGE_LOG_SIZE(periodic_log_unit);

  // Scheduling Scheme is FIFO

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
     true,
     false);
  this->initOneShotTimer = nh.createTimer(timer_options);
  this->initOneShotTimer.stop();
  // Component Timer - sensor_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.1),
     boost::bind(&sensor::sensor_timerCallback, this, _1),
     &this->compQueue, false, false);
  this->sensor_timer = nh.createTimer(timer_options);


  krpci_client.SetName(nodeName + "_" + compName);

  this->comp_sync_pub = nh.advertise<std_msgs::Bool>("component_synchronization", 1000);
  
  ros::SubscribeOptions comp_sync_sub_options;
  comp_sync_sub_options = ros::SubscribeOptions::create<std_msgs::Bool>
    ("component_synchronization",
     1000,
     boost::bind(&sensor::component_synchronization_OnOneData, this, _1),
     ros::VoidPtr(),
     &this->compQueue);
  this->comp_sync_sub = nh.subscribe(comp_sync_sub_options);

  ros::Time now = ros::Time::now();
  while ( this->comp_sync_sub.getNumPublishers() < this->num_comps_to_sync &&
	  (ros::Time::now() - now) < ros::Duration(comp_sync_timeout) )
    ros::Duration(0.1).sleep();
  ros::Duration(0.5).sleep();
  this->comp_sync_sub.shutdown();
  this->comp_sync_pub.shutdown();

  this->initOneShotTimer.start();
  this->sensor_timer.start();
  
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new sensor(config,argc,argv);
  }
}
