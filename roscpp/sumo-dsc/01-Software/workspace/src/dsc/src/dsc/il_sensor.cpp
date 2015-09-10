#include "dsc/il_sensor.hpp"


//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void il_sensor::Init(const ros::TimerEvent& event)
{
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
  initOneShotTimer.stop();
}
//# End Init Marker


// Timer Callback - il_update_timer
//# Start il_update_timerCallback Marker
void il_sensor::il_update_timerCallback(const ros::TimerEvent& event)
{
  // Business Logic for il_update_timer Timer
  dsc::sumo_il_get_vehicle_number vehicle_num;
  vehicle_num.request.sensor_name = _id;
  if ( il_get_vehicle_number_client.exists() && il_get_vehicle_number_client.call(vehicle_num))
    {
      _last_num_vehicles = vehicle_num.response.num_vehicles;
    }
  dsc::sumo_il_get_vehicle_ids vehicle_ids;
  if ( il_get_vehicle_ids_client.exists() && il_get_vehicle_ids_client.call(vehicle_ids))
    {
      _last_vehicle_ids = vehicle_ids.response.vehicle_ids;
    }
  dsc::sensor_state local_sensor_state;
  local_sensor_state.sensor_name = _id;
  local_sensor_state.num_vehicles = _last_num_vehicles;
  local_sensor_state.vehicle_ids = _last_vehicle_ids;
  sensor_state_pub.publish(local_sensor_state);
}
//# End il_update_timerCallback Marker


// Destructor - Cleanup Ports & Timers
il_sensor::~il_sensor()
{
  il_update_timer.stop();
  sensor_state_pub.shutdown();
  il_get_vehicle_ids_client.shutdown();
  il_get_vehicle_number_client.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void il_sensor::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Scheduling Scheme is FIFO

  // Component Publisher - sensor_state_pub
  advertiseName = "sensor_state";
  if (portGroupMap.find("sensor_state_pub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["sensor_state_pub"];
  this->sensor_state_pub = nh.advertise<dsc::sensor_state>(advertiseName.c_str(), 1000);

  // Configure all required services associated with this component
  // Component Client - il_get_vehicle_ids_client
  advertiseName = "sumo_il_get_vehicle_ids";
  if (portGroupMap.find("il_get_vehicle_ids_client") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["il_get_vehicle_ids_client"];
  this->il_get_vehicle_ids_client = nh.serviceClient<dsc::sumo_il_get_vehicle_ids>(advertiseName.c_str(), true); 
  // Component Client - il_get_vehicle_number_client
  advertiseName = "sumo_il_get_vehicle_number";
  if (portGroupMap.find("il_get_vehicle_number_client") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["il_get_vehicle_number_client"];
  this->il_get_vehicle_number_client = nh.serviceClient<dsc::sumo_il_get_vehicle_number>(advertiseName.c_str(), true); 

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&il_sensor::Init, this, _1),
     &this->compQueue,
     true,
     false);
  this->initOneShotTimer = nh.createTimer(timer_options);
  this->initOneShotTimer.stop();
  // Component Timer - il_update_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.1),
     boost::bind(&il_sensor::il_update_timerCallback, this, _1),
     &this->compQueue, false, false);
  this->il_update_timer = nh.createTimer(timer_options);

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


  this->comp_sync_pub = nh.advertise<std_msgs::Bool>("component_synchronization", 1000);
  
  ros::SubscribeOptions comp_sync_sub_options;
  comp_sync_sub_options = ros::SubscribeOptions::create<std_msgs::Bool>
    ("component_synchronization",
     1000,
     boost::bind(&il_sensor::component_synchronization_OnOneData, this, _1),
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
  this->il_update_timer.start();
  
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new il_sensor(config,argc,argv);
  }
}
