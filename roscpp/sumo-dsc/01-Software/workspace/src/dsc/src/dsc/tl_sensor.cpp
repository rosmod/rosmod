#include "dsc/tl_sensor.hpp"


//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void tl_sensor::Init(const ros::TimerEvent& event)
{
  // Initialize Here
  _last_state = "";
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


// Timer Callback - tl_update_timer
//# Start tl_update_timerCallback Marker
void tl_sensor::tl_update_timerCallback(const ros::TimerEvent& event)
{
  // Business Logic for tl_update_timer Timer
  
  dsc::sumo_tlc_get_ryg_state sumo_ryg_state;
  sumo_ryg_state.request.intersection_name = _id;
  if ( tlc_get_ryg_state_client.exists() && tlc_get_ryg_state_client.call(sumo_ryg_state))
    {
      _last_state = sumo_ryg_state.response.ryg_state;
    }

  dsc::ryg_state local_ryg_state;
  local_ryg_state.intersection_name = _id;
  local_ryg_state.state = _last_state;
  ryg_state_pub.publish(local_ryg_state);
}
//# End tl_update_timerCallback Marker


// Destructor - Cleanup Ports & Timers
tl_sensor::~tl_sensor()
{
  tl_update_timer.stop();
  ryg_state_pub.shutdown();
  tlc_get_ryg_state_client.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void tl_sensor::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Scheduling Scheme is FIFO

  // Component Publisher - ryg_state_pub
  advertiseName = "ryg_state";
  if (portGroupMap.find("ryg_state_pub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["ryg_state_pub"];
  this->ryg_state_pub = nh.advertise<dsc::ryg_state>(advertiseName.c_str(), 1000);

  // Configure all required services associated with this component
  // Component Client - tlc_get_ryg_state_client
  advertiseName = "sumo_tlc_get_ryg_state";
  if (portGroupMap.find("tlc_get_ryg_state_client") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["tlc_get_ryg_state_client"];
  this->tlc_get_ryg_state_client = nh.serviceClient<dsc::sumo_tlc_get_ryg_state>(advertiseName.c_str(), true); 

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&tl_sensor::Init, this, _1),
     &this->compQueue,
     true,
     false);
  this->initOneShotTimer = nh.createTimer(timer_options);
  this->initOneShotTimer.stop();
  // Component Timer - tl_update_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.1),
     boost::bind(&tl_sensor::tl_update_timerCallback, this, _1),
     &this->compQueue, false, false);
  this->tl_update_timer = nh.createTimer(timer_options);

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
     boost::bind(&tl_sensor::component_synchronization_OnOneData, this, _1),
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
  this->tl_update_timer.start();
  
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new tl_sensor(config,argc,argv);
  }
}
