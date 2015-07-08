#include "rover_pkg/high_level_controller.hpp"

KRPCI krpci_client;

//# Start User Globals Marker

#include <math.h>
#define PI 3.14159265

int current_waypoint = 0;
std::vector<Waypoint>::size_type wp_size;
Waypoint dynamicWP;

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
  return heading;
}

bool high_level_controller::isGoalReached() {

  // Check current sensor values
  if (abs(goal_heading - current_heading) < heading_tolerance ) {

    // Check last 10 sensor values
    if (previous_states.size() == 10) {
      for(boost::circular_buffer<Save_State>::iterator it = previous_states.begin(); 
	  it != previous_states.end(); ++it) {
	if (abs(goal_heading - it->heading_) > heading_tolerance)
	  return false;
      }
      return true;
    }
  }
  return false;
}

bool high_level_controller::state_func_INIT() {
  // Change State
  goal_heading = 90.0;
  goal_speed = 5.0;
  current_state = CRUISE; 
  std::vector<uint64_t> return_vec;
  //krpci_client.Control_ActivateNextStage(controlID, return_vec);
  return true;
}

bool high_level_controller::state_func_CRUISE() {
  // Set goals for LAND state
  // Iterate through all waypoints in cruise_waypoints
  if (current_waypoint < wp_size) {
    double target_lat = dynamicWP.latitude_;
    double target_lon = dynamicWP.longitude_;
    double target_lat_tolerance = dynamicWP.lat_tolerance_;
    double target_lon_tolerance = dynamicWP.long_tolerance_;

    Waypoint activeWP = cruise_waypoints[current_waypoint];    
    double wp_lat = activeWP.latitude_;
    double wp_lon = activeWP.longitude_;
    double wp_lat_tolerance = activeWP.lat_tolerance_;
    double wp_lon_tolerance = activeWP.long_tolerance_;

    double latDeltaToWP = abs(current_latitude-wp_lat);
    double lonDeltaToWP = abs(current_latitude-wp_lon);

    double latDeltaToTarget = abs(current_latitude-target_lat);
    double lonDeltaToTarget = abs(current_longitude-target_lon);

    // Find midpoint between current waypoint & next waypoint
    // Converge more gradually by choosing the midpoint between 2 waypoints!
    if (current_waypoint < wp_size) {

      // If the dynamic midpoint is very close to next waypoint, we update dynamic and current WP
      if ( latDeltaToWP < wp_lat_tolerance && lonDeltaToWP < wp_lon_tolerance) {
	current_waypoint++;
	Waypoint activeWP = cruise_waypoints[current_waypoint];    
	double wp_lat = activeWP.latitude_;
	double wp_lon = activeWP.longitude_;
	double wp_lat_tolerance = activeWP.lat_tolerance_;
	double wp_long_tolerance = activeWP.long_tolerance_;
      }
      double midpoint_latitude = (target_lat + wp_lat)/2;
      double midpoint_longitude = (target_lon + wp_lon)/2;
      goal_heading = get_relative_heading(current_latitude,
					  current_longitude,
					  midpoint_latitude,
					  midpoint_longitude);
      goal_speed = activeWP.speed_; 
	
      dynamicWP.latitude_ = midpoint_latitude;
      dynamicWP.longitude_ = midpoint_longitude;
      dynamicWP.speed_ = goal_speed;
    }
  }
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
  heading_tolerance = 5.0;
  speed_tolerance = 5.0;
  lat_tolerance = 0.02;
  long_tolerance = 0.21;

  // Starting point of CRUISE mode
  // Altitude, Latitude, Longitude, Speed, Lat. Tolerance, Long. Tolerance
  Waypoint wp1(-0.0969, -74.6004, 5.0, 0.06, 0.5);
  dynamicWP = wp1;
  cruise_waypoints.push_back(wp1);

  // Last waypoint of CRUISE mode
  Waypoint wp2(-1.5240, -71.8999, 8.0, 0.06, 0.35);
  cruise_waypoints.push_back(wp2);

  // Rover Path:
  // Lat, Long Pairs
  // -0.0490 , -74.690
  // -0.0511, -74.636
  // -0.573, -74.631
  // -0.0784, -74.618
  // -0.08465, -74.627
  // -0.08718, -74.6319
  // -0.0899, -74.633
  // -0.09255, -74.6337
  // -0.09485, -74.635
  // -0.097244, -74.6398
  // -0.101377, -74.6421
  // -0.108179, -74.6414
  // -0.11145, -74.64298
  // -0.1155, -74.6474
  // -0.119366, -74.64865
  // -0.12309, -74.6487
  // -0.12400, -74.6479
  // -0.1197, -74.6399
  // -0.1197, -74.63712
  // -0.1224, -74.63267
  // -0.1215, -74.6294
  // -0.1146, -74.6296
  // -0.1121, -74.6278
  // -0.11038, -74.6231
  // -0.11099, -74.6161
  // -0.11296, -74.6119
  // -0.1193, -74.6041
  // -0.12143, -74.5993
  // -0.12485, -74.5988
  // -0.12614, -74.5988
  // -0.127327, -74.6006
  
  wp_size = cruise_waypoints.size();

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
