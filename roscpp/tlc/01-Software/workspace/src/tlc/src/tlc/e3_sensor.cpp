#include "tlc/e3_sensor.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void e3_sensor::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering e3_sensor::init_timer_operation");
#endif
  // Initialize Here
  _last_num_vehicles = 0;
  _last_vehicle_ids = std::vector<std::string>();
  _id = "";
  for (int i=0;i<node_argc;i++)
    {
      if (!strcmp(node_argv[i],"--id"))
	{
	  _id = node_argv[i+1];
	}
    }
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting e3_sensor::init_timer_operation");
#endif  
}
//# End Init Marker


// Timer Callback - e3_update_timer
//# Start e3_update_timer_operation Marker
void e3_sensor::e3_update_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering e3_sensor::e3_update_timer_operation");
#endif
  // Business Logic for e3_update_timer_operation
  logger->log("DEBUG","Getting IL sensor state for :: %s", _id.c_str());
  tlc::e3_get_vehicle_number vehicle_num;
  vehicle_num.request.sensor_name = _id;
  if (il_get_vehicle_number_client.call(vehicle_num))
    {
      _last_num_vehicles = vehicle_num.response.num_vehicles;
    }
  else
    {
      logger->log("ERROR","client not found for IL_GET_VEHICLE_NUMBER");
    }
  tlc::e3_get_vehicle_ids vehicle_ids;
  vehicle_ids.request.sensor_name = _id;
  if ( il_get_vehicle_ids_client.call(vehicle_ids))
    {
      _last_vehicle_ids = vehicle_ids.response.vehicle_ids;
    }
  else
    {
      logger->log("ERROR","client not found for IL_GET_VEHICLE_IDS");
    }
  tlc::sensor_state local_sensor_state;
  local_sensor_state.sensor_name = _id;
  local_sensor_state.num_vehicles = _last_num_vehicles;
  local_sensor_state.vehicle_ids = _last_vehicle_ids;
  logger->log("DEBUG","Publishing IL sensor state for :: %s : %d", _id.c_str(), _last_num_vehicles);
  sensor_state_pub.publish(local_sensor_state);

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting e3_sensor::e3_update_timer_operation");
#endif
}
//# End e3_update_timer_operation Marker


// Destructor - Cleanup Ports & Timers
e3_sensor::~e3_sensor()
{
  e3_update_timer.stop();
  sensor_state_pub.shutdown();
  e3_get_vehicle_ids_client.shutdown();
  e3_get_vehicle_number_client.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void e3_sensor::startUp()
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

  // Component Publisher - sensor_state_pub
  advertiseName = "sensor_state";
  if (config.portGroupMap.find("sensor_state_pub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["sensor_state_pub"];
  this->sensor_state_pub = nh.advertise<tlc::sensor_state>(advertiseName.c_str(), 1000);

  // Configure all required services associated with this component
  // Component Client - e3_get_vehicle_ids_client
  advertiseName = "e3_get_vehicle_ids";
  if (config.portGroupMap.find("e3_get_vehicle_ids_client") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["e3_get_vehicle_ids_client"];
  this->e3_get_vehicle_ids_client = nh.serviceClient<tlc::e3_get_vehicle_ids>(advertiseName.c_str(), true); 
  // Component Client - e3_get_vehicle_number_client
  advertiseName = "e3_get_vehicle_number";
  if (config.portGroupMap.find("e3_get_vehicle_number_client") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["e3_get_vehicle_number_client"];
  this->e3_get_vehicle_number_client = nh.serviceClient<tlc::e3_get_vehicle_number>(advertiseName.c_str(), true); 

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
     boost::bind(&e3_sensor::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();
#ifdef USE_ROSMOD   
  callback_options.alias = "e3_update_timer_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif
  // Component Timer - e3_update_timer
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(0.1),
     boost::bind(&e3_sensor::e3_update_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->e3_update_timer = nh.createTimer(timer_options);



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
     boost::bind(&e3_sensor::component_sync_operation, this, _1),
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
  this->e3_update_timer.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new e3_sensor(config,argc,argv);
  }
}
