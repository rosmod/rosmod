#include "trajectory_planning_package/Trajectory_Planner.hpp"


//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Trajectory_Planner::Init(const rosmod::TimerEvent& event)
{
  compQueue.ROSMOD_LOGGER.DEBUG("Entering Trajectory_Planner::Init");
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
  compQueue.ROSMOD_LOGGER.DEBUG("Exiting Trajectory_Planner::Init");
}
//# End Init Marker

// Subscriber Callback - state_subscriber
//# Start state_subscriber_OnOneData Marker
void Trajectory_Planner::state_subscriber_OnOneData(const trajectory_planning_package::sensor_reading::ConstPtr& received_data)
{
  compQueue.ROSMOD_LOGGER.DEBUG("Entering Trajectory_Planner::state_subscriber_OnOneData");
  // Business Logic for state_subscriber Subscriber
  LOGGER.INFO("Trajectory_Planner::Subscriber::Received state: %s [%f, %f, %f]", 
	      received_data->state.c_str(),
	      received_data->x,
	      received_data->y,
	      received_data->z);

  trajectory_planning_package::compute compute_service;
  compute_service.request.x = received_data->x;
  compute_service.request.y = received_data->y;
  compute_service.request.z = received_data->z;
  if (trajectory_client.call(compute_service))
    LOGGER.INFO("Trajectory_Planner::Subscriber::Server Response - %f", compute_service.response.result);
  else {
    LOGGER.ERROR("Trajectory_Planner::Subscriber::Failed to invoke Server Operation");
    trajectory_client.waitForExistence(ros::Duration(-1));
  }
  compQueue.ROSMOD_LOGGER.DEBUG("Exiting Trajectory_Planner::state_subscriber_OnOneData");
}
//# End state_subscriber_OnOneData Marker


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
  rosmod::NodeHandle nh;
  std::string advertiseName;

  // Scheduling Scheme is FIFO
  this->compQueue.scheduling_scheme = scheduling_scheme;
  rosmod::ROSMOD_Callback_Options callback_options;

  callback_options.alias = "state_subscriber_OnOneData";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 500000000;
  // Component Subscriber - state_subscriber
  advertiseName = "sensor_reading";
  if (portGroupMap.find("state_subscriber") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["state_subscriber"];
  rosmod::SubscribeOptions state_subscriber_options;
  state_subscriber_options = rosmod::SubscribeOptions::create<trajectory_planning_package::sensor_reading>
      (advertiseName.c_str(),
       1000,
       boost::bind(&Trajectory_Planner::state_subscriber_OnOneData, this, _1),
       rosmod::VoidPtr(),
       &this->compQueue,
       callback_options);
  this->state_subscriber = nh.subscribe(state_subscriber_options);

  // Configure all required services associated with this component
  // Component Client - trajectory_client
  advertiseName = "compute";
  if (portGroupMap.find("trajectory_client") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["trajectory_client"];
      this->trajectory_client = nh.serviceClient<trajectory_planning_package::compute>(advertiseName.c_str()); 

  // Init Timer
  callback_options.alias = "Init_Timer";
  callback_options.priority = 99;
  callback_options.deadline.sec = 1;
  callback_options.deadline.nsec = 0;
  rosmod::TimerOptions timer_options;
  timer_options = 
    rosmod::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Trajectory_Planner::Init, this, _1),
     &this->compQueue,
     callback_options,
     true,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);
  this->initOneShotTimer.stop();
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
  
  rosmod::SubscribeOptions comp_sync_sub_options;
  rosmod::ROSMOD_Callback_Options sync_callback_options;
  comp_sync_sub_options = rosmod::SubscribeOptions::create<std_msgs::Bool>
    ("component_synchronization",
     1000,
     boost::bind(&Trajectory_Planner::component_synchronization_OnOneData, this, _1),
     rosmod::VoidPtr(),
     &this->compQueue,
     sync_callback_options);
  this->comp_sync_sub = nh.subscribe(comp_sync_sub_options);

  rosmod::Time now = rosmod::Time::now();
  while ( this->comp_sync_sub.getNumPublishers() < this->num_comps_to_sync &&
	  (rosmod::Time::now() - now) < rosmod::Duration(comp_sync_timeout) );
  this->comp_sync_sub.shutdown();
  this->comp_sync_pub.shutdown();

  this->initOneShotTimer.start();
  
  compQueue.ROSMOD_LOGGER.CREATE_FILE(pwd + "ROSMOD_DEBUG." + nodeName + "." + compName + ".log");
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new Trajectory_Planner(config,argc,argv);
  }
}
