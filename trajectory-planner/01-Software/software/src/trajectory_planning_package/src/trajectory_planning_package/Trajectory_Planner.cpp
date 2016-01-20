#include "trajectory_planning_package/Trajectory_Planner.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Trajectory_Planner::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering Trajectory_Planner::init_timer_operation");
#endif
  // Initialize Here
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting Trajectory_Planner::init_timer_operation");
#endif  
}
//# End Init Marker



// Subscriber Operation - state_subscriber
//# Start state_subscriber_operation Marker
void Trajectory_Planner::state_subscriber_operation(const trajectory_planning_package::sensor_reading::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering Trajectory_Planner::state_subscriber_operation");
#endif
  // Business Logic for state_subscriber_operation
  logger->log("INFO", "Trajectory_Planner::Subscriber::Received state: %s [%f, %f, %f]", 
	      received_data->state.c_str(),
	      received_data->x,
	      received_data->y,
	      received_data->z);

  trajectory_planning_package::compute compute_service;
  compute_service.request.x = received_data->x;
  compute_service.request.y = received_data->y;
  compute_service.request.z = received_data->z;
  if (trajectory_client.call(compute_service))
    logger->log("INFO", "Trajectory_Planner::Subscriber::Server Response - %f", compute_service.response.result);
  else {
    logger->log("ERROR", "Trajectory_Planner::Subscriber::Failed to invoke Server Operation");
    trajectory_client.waitForExistence(ros::Duration(-1));
  }
  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting Trajectory_Planner::state_subscriber_operation");
#endif
}
//# End state_subscriber_operation Marker


// Destructor - Cleanup Ports & Timers
Trajectory_Planner::~Trajectory_Planner()
{
  state_subscriber.shutdown();
  trajectory_client.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Trajectory_Planner::startUp()
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
     boost::bind(&Trajectory_Planner::component_sync_operation, this, _1),
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

  // Configure all subscribers associated with this component
#ifdef USE_ROSMOD 
  callback_options.alias = "state_subscriber_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 1;
  callback_options.deadline.nsec = 0;
#endif  
  // Component Subscriber - state_subscriber
  advertiseName = "sensor_reading";
  if (config.portGroupMap.find("state_subscriber") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["state_subscriber"];
  NAMESPACE::SubscribeOptions state_subscriber_options;
  state_subscriber_options = NAMESPACE::SubscribeOptions::create<trajectory_planning_package::sensor_reading>
      (advertiseName.c_str(),
       1000,
       boost::bind(&Trajectory_Planner::state_subscriber_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  this->state_subscriber = nh.subscribe(state_subscriber_options);

  // Configure all required services associated with this component
  // Component Client - trajectory_client
  advertiseName = "compute";
  if (config.portGroupMap.find("trajectory_client") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["trajectory_client"];
  this->trajectory_client = nh.serviceClient<trajectory_planning_package::compute>(advertiseName.c_str(), true); 

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
     boost::bind(&Trajectory_Planner::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();


  this->init_timer.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new Trajectory_Planner(config,argc,argv);
  }
}
