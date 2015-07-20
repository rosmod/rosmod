#include "trajectory_planning_package/Sensor.hpp"


//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Sensor::Init(const rosmod::TimerEvent& event)
{
  compQueue.ROSMOD_LOGGER.DEBUG("Entering Sensor::Init");
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
  compQueue.ROSMOD_LOGGER.DEBUG("Exiting Sensor::Init");
}
//# End Init Marker

// Server Callback - trajectory_server
//# Start computeCallback Marker
bool Sensor::computeCallback(trajectory_planning_package::compute::Request  &req,
  trajectory_planning_package::compute::Response &res)
{
  compQueue.ROSMOD_LOGGER.DEBUG("Entering Sensor::computeCallback");
  // Business Logic for trajectory_server Server
  LOGGER.INFO("Sensor::Server::Computing new result");
  float x_sq = pow(req.x, 2);
  float y_sq = pow(req.y, 2);
  float z_sq = pow(req.z, 2);
  float mean_sq = (x_sq + y_sq + z_sq)/3.0;
  res.result = sqrt(mean_sq);
  compQueue.ROSMOD_LOGGER.DEBUG("Exiting Sensor::computeCallback");
  return true;
}
//# End computeCallback Marker

// Timer Callback - sensor_timer
//# Start sensor_timerCallback Marker
void Sensor::sensor_timerCallback(const rosmod::TimerEvent& event)
{
  compQueue.ROSMOD_LOGGER.DEBUG("Entering Sensor::sensor_timerCallback");
  // Business Logic for sensor_timer Timer
  trajectory_planning_package::sensor_reading reading;
  reading.x = 5.3452;
  reading.y = 7.5209;
  reading.z = 9.1525;
  reading.state = "ERROR";
  state_publisher.publish(reading);
  compQueue.ROSMOD_LOGGER.DEBUG("Exiting Sensor::sensor_timerCallback");
}
//# End sensor_timerCallback Marker


// Destructor - Cleanup Ports & Timers
Sensor::~Sensor()
{
  sensor_timer.stop();
  state_publisher.shutdown();
  trajectory_server.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Sensor::startUp()
{
  rosmod::NodeHandle nh;
  std::string advertiseName;

  // Scheduling Scheme is FIFO
  this->compQueue.scheduling_scheme = scheduling_scheme;
  rosmod::ROSMOD_Callback_Options callback_options;

  // Component Publisher - state_publisher
  advertiseName = "sensor_reading";
  if (portGroupMap.find("state_publisher") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["state_publisher"];
  this->state_publisher = nh.advertise<trajectory_planning_package::sensor_reading>(advertiseName.c_str(), 1000);

  callback_options.alias = "computeCallback";
  callback_options.priority = 50;
  callback_options.deadline.sec =0;
  callback_options.deadline.nsec = 600000000;
  // Component Server - trajectory_server
  advertiseName = "compute";
  if (portGroupMap.find("trajectory_server") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["trajectory_server"];
  rosmod::AdvertiseServiceOptions trajectory_server_server_options;
  trajectory_server_server_options = rosmod::AdvertiseServiceOptions::create<trajectory_planning_package::compute>
      (advertiseName.c_str(),
       boost::bind(&Sensor::computeCallback, this, _1, _2),
       rosmod::VoidPtr(),
       &this->compQueue,
       callback_options);
  this->trajectory_server = nh.advertiseService(trajectory_server_server_options);
 
  // Init Timer
  callback_options.alias = "Init_Timer";
  callback_options.priority = 99;
  callback_options.deadline.sec = 1;
  callback_options.deadline.nsec = 0;
  rosmod::TimerOptions timer_options;
  timer_options = 
    rosmod::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Sensor::Init, this, _1),
     &this->compQueue,
     callback_options,
     true,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);
  this->initOneShotTimer.stop();
  callback_options.alias = "sensor_timerCallback";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 400000000;
  // Component Timer - sensor_timer
  timer_options = 
    rosmod::TimerOptions
    (ros::Duration(1.0),
     boost::bind(&Sensor::sensor_timerCallback, this, _1),
     &this->compQueue,
     callback_options,
     false,
     false);
  this->sensor_timer = nh.createTimer(timer_options);

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
     boost::bind(&Sensor::component_synchronization_OnOneData, this, _1),
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
  this->sensor_timer.start();
  
  compQueue.ROSMOD_LOGGER.CREATE_FILE(pwd + "ROSMOD_DEBUG." + nodeName + "." + compName + ".log");
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new Sensor(config,argc,argv);
  }
}
