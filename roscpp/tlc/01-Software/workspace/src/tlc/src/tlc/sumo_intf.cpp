#include "tlc/sumo_intf.hpp"

SUMO_CLIENT sumo_client;
//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void sumo_intf::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering sumo_intf::init_timer_operation");
#endif
  // Initialize Here
  std::string sumo_host;
  int sumo_port;
  for (int i=0; i<node_argc; i++)
    {
      if (!strcmp(node_argv[i], "--sumo_host"))
	{
	  sumo_host = node_argv[i+1];
	}
      if (!strcmp(node_argv[i], "--sumo_port"))
	{
	  sumo_port = atoi(node_argv[i+1]);
	}
    }
  sumo_client.create_connection(sumo_port, sumo_host);
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting sumo_intf::init_timer_operation");
#endif  
}
//# End Init Marker


// Server Operation - e3_get_vehicle_number_server
//# Start e3_get_vehicle_number_operation Marker
bool sumo_intf::e3_get_vehicle_number_operation(tlc::e3_get_vehicle_number::Request  &req,
  tlc::e3_get_vehicle_number::Response &res)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering sumo_intf::e3_get_vehicle_number_operation");
#endif
  // Business Logic for e3_get_vehicle_number_server_operation
  if ( _e3_num_vehicles_map.find( req.sensor_name ) == _e3_num_vehicles_map.end() )
    {
      _e3_num_vehicles_map[ req.sensor_name ] =
	sumo_client.multientryexit.getLastStepVehicleNumber( req.sensor_name );
    }
  res.num_vehicles = _e3_num_vehicles_map[ req.sensor_name ];
  _e3_num_vehicles_map[ req.sensor_name ] = 0;

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting sumo_intf::e3_get_vehicle_number_operation");
#endif
  return true;
}
//# End e3_get_vehicle_number_operation Marker
// Server Operation - e3_get_vehicle_ids_server
//# Start e3_get_vehicle_ids_operation Marker
bool sumo_intf::e3_get_vehicle_ids_operation(tlc::e3_get_vehicle_ids::Request  &req,
  tlc::e3_get_vehicle_ids::Response &res)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering sumo_intf::e3_get_vehicle_ids_operation");
#endif
  // Business Logic for e3_get_vehicle_ids_server_operation
  if ( _e3_vehicle_ids_map.find( req.sensor_name ) == _e3_vehicle_ids_map.end() )
    {
      _e3_vehicle_ids_map[ req.sensor_name ] =
	sumo_client.multientryexit.getLastStepVehicleIDs( req.sensor_name );
    }
  res.vehicle_ids = _e3_vehicle_ids_map[ req.sensor_name ];
  _e3_vehicle_ids_map[ req.sensor_name ] = std::vector<std::string>();

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting sumo_intf::e3_get_vehicle_ids_operation");
#endif
  return true;
}
//# End e3_get_vehicle_ids_operation Marker
// Server Operation - tlc_get_ryg_state_server
//# Start tlc_get_ryg_state_operation Marker
bool sumo_intf::tlc_get_ryg_state_operation(tlc::tlc_get_ryg_state::Request  &req,
  tlc::tlc_get_ryg_state::Response &res)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering sumo_intf::tlc_get_ryg_state_operation");
#endif
  // Business Logic for tlc_get_ryg_state_server_operation
  if ( _tl_state_map.find( req.intersection_name ) == _tl_state_map.end() )
    {
      _tl_state_map[ req.intersection_name ] =
	sumo_client.trafficlights.getRedYellowGreenState( req.intersection_name );
    }
  res.ryg_state = _tl_state_map[ req.intersection_name ];

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting sumo_intf::tlc_get_ryg_state_operation");
#endif
  return true;
}
//# End tlc_get_ryg_state_operation Marker
// Server Operation - tlc_set_ryg_state_server
//# Start tlc_set_ryg_state_operation Marker
bool sumo_intf::tlc_set_ryg_state_operation(tlc::tlc_set_ryg_state::Request  &req,
  tlc::tlc_set_ryg_state::Response &res)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering sumo_intf::tlc_set_ryg_state_operation");
#endif
  // Business Logic for tlc_set_ryg_state_server_operation
  sumo_client.trafficlights.setRedYellowGreenState( req.intersection_name, req.ryg_state );

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting sumo_intf::tlc_set_ryg_state_operation");
#endif
  return true;
}
//# End tlc_set_ryg_state_operation Marker

// Timer Callback - sumo_step_timer
//# Start sumo_step_timer_operation Marker
void sumo_intf::sumo_step_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
	  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering sumo_intf::sumo_step_timer_operation");
#endif
  // Business Logic for sumo_step_timer_operation
  // STEP THE SIMULATION
  sumo_client.commandSimulationStep(0);
  // UPDATE ALL SENSOR DATA
  for (auto it = _e3_num_vehicles_map.begin(); it != _e3_num_vehicles_map.end(); ++it)
    {
      it->second =
	sumo_client.multientryexit.getLastStepVehicleNumber( it->first );
    }
  for (auto it = _e3_vehicle_ids_map.begin(); it != _e3_vehicle_ids_map.end(); ++it)
    {
      it->second =
	sumo_client.multientryexit.getLastStepVehicleIDs( it->first );
    }
  for (auto it = _tl_state_map.begin(); it != _tl_state_map.end(); ++it)
    {
      it->second = 
	sumo_client.trafficlights.getRedYellowGreenState( it->first );
    }

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting sumo_intf::sumo_step_timer_operation");
#endif
}
//# End sumo_step_timer_operation Marker


// Destructor - Cleanup Ports & Timers
sumo_intf::~sumo_intf()
{
  sumo_step_timer.stop();
  e3_get_vehicle_number_server.shutdown();
  e3_get_vehicle_ids_server.shutdown();
  tlc_get_ryg_state_server.shutdown();
  tlc_set_ryg_state_server.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void sumo_intf::startUp()
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

#ifdef USE_ROSMOD  
  callback_options.alias = "e3_get_vehicle_number_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec =0;
  callback_options.deadline.nsec = 100000000;
#endif    
  // Component Server - e3_get_vehicle_number_server
  advertiseName = "e3_get_vehicle_number";
  if (config.portGroupMap.find("e3_get_vehicle_number_server") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["e3_get_vehicle_number_server"];
  NAMESPACE::AdvertiseServiceOptions e3_get_vehicle_number_server_server_options;
  e3_get_vehicle_number_server_server_options = NAMESPACE::AdvertiseServiceOptions::create<tlc::e3_get_vehicle_number>
      (advertiseName.c_str(),
       boost::bind(&sumo_intf::e3_get_vehicle_number_operation, this, _1, _2),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD       
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif
  this->e3_get_vehicle_number_server = nh.advertiseService(e3_get_vehicle_number_server_server_options);
#ifdef USE_ROSMOD  
  callback_options.alias = "e3_get_vehicle_ids_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec =0;
  callback_options.deadline.nsec = 100000000;
#endif    
  // Component Server - e3_get_vehicle_ids_server
  advertiseName = "e3_get_vehicle_ids";
  if (config.portGroupMap.find("e3_get_vehicle_ids_server") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["e3_get_vehicle_ids_server"];
  NAMESPACE::AdvertiseServiceOptions e3_get_vehicle_ids_server_server_options;
  e3_get_vehicle_ids_server_server_options = NAMESPACE::AdvertiseServiceOptions::create<tlc::e3_get_vehicle_ids>
      (advertiseName.c_str(),
       boost::bind(&sumo_intf::e3_get_vehicle_ids_operation, this, _1, _2),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD       
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif
  this->e3_get_vehicle_ids_server = nh.advertiseService(e3_get_vehicle_ids_server_server_options);
#ifdef USE_ROSMOD  
  callback_options.alias = "tlc_get_ryg_state_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec =0;
  callback_options.deadline.nsec = 100000000;
#endif    
  // Component Server - tlc_get_ryg_state_server
  advertiseName = "tlc_get_ryg_state";
  if (config.portGroupMap.find("tlc_get_ryg_state_server") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["tlc_get_ryg_state_server"];
  NAMESPACE::AdvertiseServiceOptions tlc_get_ryg_state_server_server_options;
  tlc_get_ryg_state_server_server_options = NAMESPACE::AdvertiseServiceOptions::create<tlc::tlc_get_ryg_state>
      (advertiseName.c_str(),
       boost::bind(&sumo_intf::tlc_get_ryg_state_operation, this, _1, _2),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD       
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif
  this->tlc_get_ryg_state_server = nh.advertiseService(tlc_get_ryg_state_server_server_options);
#ifdef USE_ROSMOD  
  callback_options.alias = "tlc_set_ryg_state_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec =0;
  callback_options.deadline.nsec = 100000000;
#endif    
  // Component Server - tlc_set_ryg_state_server
  advertiseName = "tlc_set_ryg_state";
  if (config.portGroupMap.find("tlc_set_ryg_state_server") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["tlc_set_ryg_state_server"];
  NAMESPACE::AdvertiseServiceOptions tlc_set_ryg_state_server_server_options;
  tlc_set_ryg_state_server_server_options = NAMESPACE::AdvertiseServiceOptions::create<tlc::tlc_set_ryg_state>
      (advertiseName.c_str(),
       boost::bind(&sumo_intf::tlc_set_ryg_state_operation, this, _1, _2),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD       
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif
  this->tlc_set_ryg_state_server = nh.advertiseService(tlc_set_ryg_state_server_server_options);
 
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
     boost::bind(&sumo_intf::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();
#ifdef USE_ROSMOD   
  callback_options.alias = "sumo_step_timer_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif
  // Component Timer - sumo_step_timer
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(0.1),
     boost::bind(&sumo_intf::sumo_step_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->sumo_step_timer = nh.createTimer(timer_options);



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
     boost::bind(&sumo_intf::component_sync_operation, this, _1),
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

  this->init_timer.start();
  this->sumo_step_timer.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new sumo_intf(config,argc,argv);
  }
}
