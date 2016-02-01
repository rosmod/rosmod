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
void sensor::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering sensor::init_timer_operation");
#endif
  // Initialize Here
  std::string ksp_host;
  int ksp_port;
  for (int i=0; i<node_argc; i++)
    {
      if (!strcmp(node_argv[i], "--ksp_host"))
	{
	  ksp_host = node_argv[i+1];
	}
      if (!strcmp(node_argv[i], "--ksp_port"))
	{
	  ksp_port = atoi(node_argv[i+1]);
	}
    }
  krpci_client.SetPort(ksp_port);
  krpci_client.SetIP(ksp_host);
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
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting sensor::init_timer_operation");
#endif  
}
//# End Init Marker


// Timer Callback - sensor_timer
//# Start sensor_timer_operation Marker
void sensor::sensor_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering sensor::sensor_timer_operation");
#endif
  // Business Logic for sensor_timer_operation
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

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting sensor::sensor_timer_operation");
#endif
}
//# End sensor_timer_operation Marker


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
  NAMESPACE::NodeHandle nh;
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
  std::string log_file_path = pwd + config.nodeName + "." + config.compName + ".log"; 

  logger->create_file(pwd + config.nodeName + "." + config.compName + ".log");
  logger->set_is_periodic(config.is_periodic_logging);
  logger->set_max_log_unit(config.periodic_log_unit);

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->create_file(pwd + "ROSMOD_DEBUG." + config.nodeName + "." + config.compName + ".log");
  comp_queue.ROSMOD_LOGGER->set_is_periodic(config.is_periodic_logging);
  comp_queue.ROSMOD_LOGGER->set_max_log_unit(config.periodic_log_unit);
#endif    
  
#ifdef USE_ROSMOD 
  this->comp_queue.scheduling_scheme = config.schedulingScheme;
  rosmod::ROSMOD_Callback_Options callback_options;
#endif  
  // Configure all publishers associated with this component
  // Component Publisher - vessel_state_pub
  advertiseName = "vessel_state";
  if (config.portGroupMap.find("vessel_state_pub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["vessel_state_pub"];
  this->vessel_state_pub = nh.advertise<rover_pkg::vessel_state>(advertiseName.c_str(), 1000);


  // Synchronize components now that all publishers and servers have been initialized
  this->comp_sync_pub = nh.advertise<std_msgs::Bool>("component_synchronization", 1000);
  
#ifdef USE_ROSMOD  
  rosmod::SubscribeOptions comp_sync_sub_options;
  rosmod::ROSMOD_Callback_Options sync_callback_options;
#else
  ros::SubscribeOptions comp_sync_sub_options;
#endif
  
  comp_sync_sub_options = NAMESPACE::SubscribeOptions::create<std_msgs::Bool>
    ("component_synchronization",
     1000,
     boost::bind(&sensor::component_sync_operation, this, _1),
     NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD     
     &this->comp_queue,
     sync_callback_options);
#else
     &this->comp_queue);
#endif
  this->comp_sync_sub = nh.subscribe(comp_sync_sub_options);

  ros::Time now = ros::Time::now();
  while ( this->comp_sync_sub.getNumPublishers() < this->config.num_comps_to_sync &&
	  (ros::Time::now() - now) < ros::Duration(config.comp_sync_timeout))
  ros::Duration(0.1).sleep();
  ros::Duration(0.5).sleep();
  this->comp_sync_sub.shutdown();  
  this->comp_sync_pub.shutdown();


  // Init Timer
#ifdef USE_ROSMOD    
  callback_options.alias = "init_timer_operation";
  callback_options.priority = 99;
  callback_options.deadline.sec = 1;
  callback_options.deadline.nsec = 0;
#endif
  NAMESPACE::TimerOptions timer_options;
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(-1),
     boost::bind(&sensor::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();
#ifdef USE_ROSMOD   
  callback_options.alias = "sensor_timer_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif
  // Component Timer - sensor_timer
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(0.1),
     boost::bind(&sensor::sensor_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->sensor_timer = nh.createTimer(timer_options);

  krpci_client.SetName(config.nodeName + "_" + config.compName);

  this->init_timer.start();
  this->sensor_timer.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new sensor(config,argc,argv);
  }
}
