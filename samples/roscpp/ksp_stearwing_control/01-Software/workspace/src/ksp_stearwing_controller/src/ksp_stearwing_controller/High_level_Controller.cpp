#include "ksp_stearwing_controller/High_level_Controller.hpp"

KRPCI krpci_client;

//# Start User Globals Marker
#include <math.h>
#define PI 3.14159265

int current_waypoint = 0;
std::vector<Waypoint>::size_type wp_size; 

uint64_t vesselID;
uint64_t controlID;
uint64_t surfaceRefFrameID;
double cruise_altitude;
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

bool High_level_Controller::isGoalReached() {

  // Check current sensor values
  if ((abs(goal_heading - current_heading) < heading_tolerance) &&
      (abs(goal_mean_altitude - current_mean_altitude) < mean_altitude_tolerance)) {

    // Check last 10 sensor values
    if (previous_states.size() == 10) {
      for(boost::circular_buffer<Save_State>::iterator it = previous_states.begin(); 
	  it != previous_states.end(); ++it) {
	  if ((abs(goal_heading - it->heading_) > heading_tolerance) &&
	      (abs(goal_mean_altitude - it->altitude_) > mean_altitude_tolerance))
	    return false;
      }
      return true;
    }
  }
  return false;
}

bool High_level_Controller::state_func_INIT() {
  // Change State
  goal_mean_altitude = cruise_altitude;
  goal_heading = 90.0;
  goal_speed = 180.0;
  current_state = TAKE_OFF; 
  std::vector<uint64_t> return_vec;
  krpci_client.Control_ActivateNextStage(controlID, return_vec);
  return true;
}

bool High_level_Controller::state_func_TAKEOFF() {
  // Change State
  if (isGoalReached())
    current_state = CRUISE;
  return true;
}

bool High_level_Controller::state_func_CRUISE() {
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
    goal_mean_altitude = activeWP.altitude_;
    goal_speed = activeWP.speed_; 
  }
  else
    current_waypoint = 0;
}

bool High_level_Controller::state_func_LAND() {
  // Time to Land
  // Without throttle control, this is going to be rough
}

//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void High_level_Controller::Init(const ros::TimerEvent& event)
{
  // Initialize Here
  goal_mean_altitude = 70.0;
  goal_heading = 90.0;
  current_state = INIT;

  cruise_altitude = 500.0f;
  for (int i=0;i<node_argc;i++)
    {
      if (!strcmp(node_argv[i],"--cruise_alt"))
	{
	  cruise_altitude = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--krpc_ip"))
	{
	  krpci_client.SetIP(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--krpc_port"))
	{
	  krpci_client.SetPort(atoi(node_argv[i+1]));	  
	}      
    }

  // Set the history capacity
  previous_states.set_capacity(10);

  // Set Goal tolerances
  heading_tolerance = 5.0;
  mean_altitude_tolerance = 10.0;
  speed_tolerance = 5.0;

  lat_tolerance = 0.02;
  long_tolerance = 0.21;

  // Starting point of CRUISE mode
  // Altitude, Latitude, Longitude, Speed, Lat. Tolerance, Long. Tolerance
  Waypoint wp1(500.0, -0.5718, -73.75879, 180.0, 0.06, 0.1);
  Waypoint wp2(500.0, -1.2979, -72.8931, 180.0, 0.06, 0.1);
  Waypoint wp3(300.0, -1.5187, -72.1472, 150.0, 0.03, 0.1);
  Waypoint wp4(300.0, -1.5157, -71.9859, 0.0, 0.03, 0.1);
  Waypoint wp5(150.0, -1.5157, -71.89, 0.0, 0.005, 0.05);
  Waypoint wp6(150.0, -1.5157, -71.89, 0.0, 0.005, 0.05);
  cruise_waypoints.push_back(wp1);
  cruise_waypoints.push_back(wp2);
  cruise_waypoints.push_back(wp3);
  cruise_waypoints.push_back(wp4);
  cruise_waypoints.push_back(wp6);
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
  current_mean_altitude = received_data->mean_altitude;
  current_latitude = received_data->latitude;
  current_longitude = received_data->longitude;
  current_speed = received_data->speed;
  current_surface_altitude = received_data->surface_altitude;
  current_landing_gear = received_data->landing_gear;
  Save_State new_state(current_heading, current_mean_altitude, current_speed);
  previous_states.push_back(new_state);
}
//# End sensor_subscriber_OnOneData Marker

// Timer Callback - flight_control_timer
//# Start flight_control_timerCallback Marker
void High_level_Controller::flight_control_timerCallback(const ros::TimerEvent& event)
{
  // Business Logic for flight_control_timer Timer
  if (current_surface_altitude > 150)// && !current_landing_gear)
    krpci_client.Control_set_Gear(controlID, 0);
  else if (current_surface_altitude < 150) {// && current_landing_gear) 
    krpci_client.Control_set_Gear(controlID, 1);
    if (current_state != INIT && current_state != TAKE_OFF) 
      krpci_client.Control_set_Brakes(controlID, 1);
  }
  
  switch(current_state) {
  case INIT :
    state_func_INIT();
    break;
  case TAKE_OFF :
    state_func_TAKEOFF();
    break;
  case CRUISE :
    state_func_CRUISE();
    break;
  case LAND :
    state_func_LAND();
    break;
  }

  // Green heading marker
  krpci_client.ClearDrawing();
  double x=0, y, z;
  z = sin(goal_heading * PI/180.0);
  y = cos(goal_heading * PI/180.0);
  krpci_client.DrawDirection(x,y,z,surfaceRefFrameID,0,1,0,30);

  // Publish newly set goals
  ksp_stearwing_controller::Control_Command new_command;
  new_command.goal_altitude = goal_mean_altitude;
  new_command.goal_speed = goal_speed;
  new_command.goal_heading = goal_heading;
  pid_control_publisher.publish(new_command);
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
  LOGGER.DEBUG("Entering High_level_Controller::startUp");
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
  LOGGER.DEBUG("Exiting High_level_Controller::startUp");
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new High_level_Controller(config,argc,argv);
  }
}
