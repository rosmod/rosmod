#include "rover_pkg/high_level_controller.hpp"

KRPCI krpci_client;

//# Start User Globals Marker

#include <math.h>
#define PI 3.14159265

int current_waypoint = 0;
std::vector<Waypoint>::size_type wp_size;
Waypoint dynamicWP;

double cruising_speed = 20.0;
double waypoint_latitude_tolerance = 0.005;
double waypoint_longitude_tolerance = 0.005;

uint64_t vesselID;
uint64_t controlID;
uint64_t surfaceRefFrameID;
uint64_t orbitID;
uint64_t bodyID;
uint64_t orbitalRefFrameID;

float get_relative_heading(double current_latitude, 
			   double current_longitude,
			   double target_latitude,
			   double target_longitude) {
  double relative_longitude = target_longitude - current_longitude;
  double relative_latitude = target_latitude - current_latitude;
  float heading = (90.0 - atan2(relative_latitude, relative_longitude) * 180/PI);
  if (heading < 0)
    heading = 360 + heading;
  if (heading > 360.0)
    heading = heading - 360;
  return heading;
}

bool high_level_controller::isGoalReached() {
  // Check current sensor values
  double headingDiff = abs(goal_heading - current_heading);
  if ( headingDiff < heading_tolerance ) {
    return true;
  }
  return false;
}

bool high_level_controller::state_func_INIT() {
  // Change State
  goal_heading = 90.0;
  goal_speed = 15.0;
  current_state = CRUISE; 
  std::vector<uint64_t> return_vec;
  //krpci_client.Control_ActivateNextStage(controlID, return_vec);
  return true;
}

bool high_level_controller::state_func_CRUISE() {
  // Set goals for LAND state
  // Iterate through all waypoints in cruise_waypoints
  Waypoint activeWP = cruise_waypoints[current_waypoint];    
  double wp_lat = activeWP.latitude_;
  double wp_lon = activeWP.longitude_;
  double wp_lat_tolerance = activeWP.lat_tolerance_;
  double wp_lon_tolerance = activeWP.long_tolerance_;

  // Find midpoint between current waypoint & next waypoint
  // Converge more gradually by choosing the midpoint between 2 waypoints!
  if (current_waypoint < wp_size) {

    double latDiff = abs(current_latitude - wp_lat);
    double lonDiff = abs(current_longitude - wp_lon);

    // If the dynamic midpoint is very close to next waypoint, we update dynamic and current WP
    if ( latDiff < wp_lat_tolerance && lonDiff < wp_lon_tolerance) {
      current_waypoint++;
      activeWP = cruise_waypoints[current_waypoint];    
      wp_lat = activeWP.latitude_;
      wp_lon = activeWP.longitude_;
    }
    double midpoint_latitude = (current_latitude + wp_lat)/2;
    double midpoint_longitude = (current_longitude + wp_lon)/2;
    goal_heading = get_relative_heading(current_latitude,
					current_longitude,
					midpoint_latitude,
					midpoint_longitude);
    goal_speed = activeWP.speed_; 
  }
  else
    current_waypoint = 0;
}

//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void high_level_controller::Init(const ros::TimerEvent& event)
{
  LOGGER.DEBUG("Entering high_level_controller::Init");
  // Initialize Here
  goal_heading = 90.0;
  current_state = INIT;
  
  // Set the history capacity
  previous_states.set_capacity(10);

  // Set Goal tolerances
  heading_tolerance = 1.0;
  speed_tolerance = 1.0;
  lat_tolerance = 0.01;
  long_tolerance = 0.02;
  
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
      if (!strcmp(node_argv[i],"--cruising_speed"))
	{
	  cruising_speed = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--lat_tolerance"))
	{
	  lat_tolerance = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--long_tolerance"))
	{
	  long_tolerance = atof(node_argv[i+1]);
	}
    }

  // Starting point of CRUISE mode
  // Latitude, Longitude, Speed, Lat. Tolerance, Long. Tolerance
  // Rover Path:
  Waypoint wp1(-0.0490 , -74.690,
	       cruising_speed,
	       waypoint_latitude_tolerance,
	       waypoint_longitude_tolerance);
  dynamicWP = wp1;
  cruise_waypoints.push_back(wp1);
  Waypoint wp2(-0.0511, -74.636,
	       cruising_speed,
	       waypoint_latitude_tolerance,
	       waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp2);
  Waypoint wp3(-0.0573, -74.631,
	       cruising_speed,
	       waypoint_latitude_tolerance,
	       waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp3);
  Waypoint wp4(-0.0784, -74.618,
	       cruising_speed,
	       waypoint_latitude_tolerance,
	       waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp4);
  Waypoint wp5(-0.08465, -74.627,
	       cruising_speed,
	       waypoint_latitude_tolerance,
	       waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp5);
  Waypoint wp6(-0.08718, -74.6319,
	       cruising_speed,
	       waypoint_latitude_tolerance,
	       waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp6);
  Waypoint wp7(-0.0899, -74.633,
	       cruising_speed,
	       waypoint_latitude_tolerance,
	       waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp7);
  Waypoint wp8(-0.09255, -74.6337,
	       cruising_speed,
	       waypoint_latitude_tolerance,
	       waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp8);
  Waypoint wp9(-0.09485, -74.635,
	       cruising_speed,
	       waypoint_latitude_tolerance,
	       waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp9);
  Waypoint wp10(-0.097244, -74.6398,
		cruising_speed,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp10);
  Waypoint wp11(-0.101377, -74.6421,
		cruising_speed,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp11);
  Waypoint wp12(-0.108179, -74.6414,
		cruising_speed,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp12);
  Waypoint wp13(-0.11145, -74.64298,
		cruising_speed,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp13);
  Waypoint wp14(-0.1155, -74.6474,
		cruising_speed,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp14);
  Waypoint wp15(-0.119366, -74.64865,
		cruising_speed,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp15);
  Waypoint wp16(-0.12309, -74.6487,
		cruising_speed,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp16);
  Waypoint wp17(-0.12400, -74.6479,
		cruising_speed,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp17);
  Waypoint wp18(-0.1197, -74.6399,
		cruising_speed,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp18);
  Waypoint wp19(-0.1197, -74.63712,
		cruising_speed,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp19);
  Waypoint wp20(-0.1224, -74.63267,
		cruising_speed,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp20);
  Waypoint wp21(-0.1215, -74.6294,
		cruising_speed,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp21);
  Waypoint wp22(-0.1146, -74.6296,
		cruising_speed,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp22);
  Waypoint wp23(-0.1121, -74.6278,
		cruising_speed,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp23);
  Waypoint wp24(-0.11038, -74.6231,
		cruising_speed,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp24);
  Waypoint wp25(-0.11099, -74.6161,
		cruising_speed,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp25);
  Waypoint wp26(-0.11296, -74.6119,
		cruising_speed,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp26);
  Waypoint wp27(-0.1193, -74.6041,
		cruising_speed,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp27);
  Waypoint wp28(-0.12143, -74.5993,
		cruising_speed,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp28);
  Waypoint wp29(-0.12485, -74.5988,
		cruising_speed,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp29);
  Waypoint wp30(-0.12614, -74.5988,
		cruising_speed,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp30);
  Waypoint wp31(-0.127327, -74.6006,
		cruising_speed,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp31);
  
  wp_size = cruise_waypoints.size();

  // Connect to kRPC Server and obtain the vessel & control ID
  if (krpci_client.Connect()) {
    krpci_client.get_ActiveVessel(vesselID);
    krpci_client.Vessel_get_Control(vesselID, controlID);
    krpci_client.Vessel_get_SurfaceReferenceFrame(vesselID, surfaceRefFrameID);
    krpci_client.Vessel_get_Orbit(vesselID, orbitID);
    krpci_client.Orbit_get_Body(orbitID, bodyID);
    krpci_client.CelestialBody_get_ReferenceFrame(bodyID, orbitalRefFrameID);
  }

  // Stop Init Timer
  initOneShotTimer.stop();
  LOGGER.DEBUG("Exiting high_level_controller::Init");
}
//# End Init Marker

// Subscriber Callback - vessel_state_sub
//# Start vessel_state_sub_OnOneData Marker
void high_level_controller::vessel_state_sub_OnOneData(const rover_pkg::vessel_state::ConstPtr& received_data)
{
  LOGGER.DEBUG("Entering high_level_controller::vessel_state_sub_OnOneData");
  // Business Logic for vessel_state_sub Subscriber
  current_heading = received_data->heading;
  current_latitude = received_data->latitude;
  current_longitude = received_data->longitude;
  current_speed = received_data->speed;
  current_landing_gear = received_data->landing_gear;
  current_brakes = received_data->brakes;
  Save_State new_state(current_heading, current_speed);
  previous_states.push_back(new_state);

  LOGGER.DEBUG("Exiting high_level_controller::vessel_state_sub_OnOneData");
}
//# End vessel_state_sub_OnOneData Marker

// Timer Callback - state_timer
//# Start state_timerCallback Marker
void high_level_controller::state_timerCallback(const ros::TimerEvent& event)
{
  LOGGER.DEBUG("Entering high_level_controller::state_timerCallback");
  // Business Logic for state_timer Timer
  switch(current_state) {
  case INIT :
    state_func_INIT();
    break;
  case CRUISE :
    state_func_CRUISE();
    break;
  }

  // Green heading marker
  krpci_client.ClearDirections();
  double x=0, y, z;
  z = sin(goal_heading * PI/180.0);
  y = cos(goal_heading * PI/180.0);
  krpci_client.DrawDirection(x,y,z,surfaceRefFrameID,0,1,0,30);

  // Publish newly set goals
  rover_pkg::goal_state new_state;
  new_state.goal_speed = goal_speed;
  new_state.goal_heading = goal_heading;
  goal_state_pub.publish(new_state);

  LOGGER.DEBUG("Exiting high_level_controller::state_timerCallback");
}
//# End state_timerCallback Marker


// Destructor - Cleanup Ports & Timers
high_level_controller::~high_level_controller()
{
  state_timer.stop();
  goal_state_pub.shutdown();
  vessel_state_sub.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void high_level_controller::startUp()
{
  LOGGER.DEBUG("Entering high_level_controller::startUp");
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Subscriber - vessel_state_sub
  advertiseName = "vessel_state";
  if (portGroupMap.find("vessel_state_sub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["vessel_state_sub"];
  ros::SubscribeOptions vessel_state_sub_options;
  vessel_state_sub_options = ros::SubscribeOptions::create<rover_pkg::vessel_state>
    (advertiseName.c_str(),
     1000,
     boost::bind(&high_level_controller::vessel_state_sub_OnOneData, this, _1),
     ros::VoidPtr(),
     &this->compQueue);
  this->vessel_state_sub = nh.subscribe(vessel_state_sub_options);

  // Component Publisher - goal_state_pub
  advertiseName = "goal_state";
  if (portGroupMap.find("goal_state_pub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["goal_state_pub"];
  this->goal_state_pub = nh.advertise<rover_pkg::goal_state>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&high_level_controller::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - state_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.5),
     boost::bind(&high_level_controller::state_timerCallback, this, _1),
     &this->compQueue);
  this->state_timer = nh.createTimer(timer_options);

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
  LOGGER.DEBUG("Exiting high_level_controller::startUp");
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new high_level_controller(config,argc,argv);
  }
}
