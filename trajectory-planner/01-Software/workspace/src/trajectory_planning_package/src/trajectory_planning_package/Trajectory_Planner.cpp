#include "trajectory_planning_package/Trajectory_Planner.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Trajectory_Planner::Init(const ros::TimerEvent& event)
{
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Subscriber Callback - state_subscriber
//# Start state_subscriber_OnOneData Marker
void Trajectory_Planner::state_subscriber_OnOneData(const trajectory_planning_package::sensor_reading::ConstPtr& received_data)
{
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
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Subscriber - state_subscriber
  advertiseName = "sensor_reading";
  if (portGroupMap.find("state_subscriber") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["state_subscriber"];
  ros::SubscribeOptions state_subscriber_options;
  state_subscriber_options = ros::SubscribeOptions::create<trajectory_planning_package::sensor_reading>
      (advertiseName.c_str(),
       1000,
       boost::bind(&Trajectory_Planner::state_subscriber_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->state_subscriber = nh.subscribe(state_subscriber_options);

  // Configure all required services associated with this component
  // Component Client - trajectory_client
  advertiseName = "compute";
  if (portGroupMap.find("trajectory_client") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["trajectory_client"];
      this->trajectory_client = nh.serviceClient<trajectory_planning_package::compute>(advertiseName.c_str()); 

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Trajectory_Planner::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
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
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new Trajectory_Planner(config,argc,argv);
  }
}
