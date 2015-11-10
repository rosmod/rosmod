#include "timer_package/Timer.hpp"


//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Timer::Init(const rosmod::TimerEvent& event)
{
  compQueue.ROSMOD_LOGGER.DEBUG("Entering Timer::Init");
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
  compQueue.ROSMOD_LOGGER.DEBUG("Exiting Timer::Init");
}
//# End Init Marker


// Timer Callback - periodic_timer
//# Start periodic_timerCallback Marker
void Timer::periodic_timerCallback(const rosmod::TimerEvent& event)
{
  compQueue.ROSMOD_LOGGER.DEBUG("Entering Timer::periodic_timerCallback");
  // Business Logic for periodic_timer Timer

  compQueue.ROSMOD_LOGGER.DEBUG("Exiting Timer::periodic_timerCallback");
}
//# End periodic_timerCallback Marker
// Timer Callback - sporadic_timer
//# Start sporadic_timerCallback Marker
void Timer::sporadic_timerCallback(const rosmod::TimerEvent& event)
{
  compQueue.ROSMOD_LOGGER.DEBUG("Entering Timer::sporadic_timerCallback");
  // Business Logic for sporadic_timer Timer

  compQueue.ROSMOD_LOGGER.DEBUG("Exiting Timer::sporadic_timerCallback");
}
//# End sporadic_timerCallback Marker


// Destructor - Cleanup Ports & Timers
Timer::~Timer()
{
  periodic_timer.stop();
  sporadic_timer.stop();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Timer::startUp()
{
  rosmod::NodeHandle nh;
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
  std::string log_file_path = pwd + nodeName + "." + compName + ".log"; 
  
  // Create the log file & open file stream
  LOGGER.CREATE_FILE(log_file_path);
  
  // Establish log levels of LOGGER
  LOGGER.SET_LOG_LEVELS(logLevels);

  // Prepare logging periodicity
  LOGGER.SET_PERIODICITY(is_periodic_logging);
  LOGGER.CHANGE_LOG_SIZE(periodic_log_unit);

  // Scheduling Scheme is FIFO
  this->compQueue.scheduling_scheme = scheduling_scheme;
  rosmod::ROSMOD_Callback_Options callback_options;

  // Init Timer
  callback_options.alias = "Init_Timer";
  callback_options.priority = 99;
  callback_options.deadline.sec = 1;
  callback_options.deadline.nsec = 0;
  rosmod::TimerOptions timer_options;
  timer_options = 
    rosmod::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Timer::Init, this, _1),
     &this->compQueue,
     callback_options,
     true,
     false);
  this->initOneShotTimer = nh.createTimer(timer_options);
  this->initOneShotTimer.stop();
  callback_options.alias = "periodic_timerCallback";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 500000000;
  // Component Timer - periodic_timer
  timer_options = 
    rosmod::TimerOptions
    (ros::Duration(1.0),
     boost::bind(&Timer::periodic_timerCallback, this, _1),
     &this->compQueue,
     callback_options,
     false,
     false);
  this->periodic_timer = nh.createTimer(timer_options);
  callback_options.alias = "sporadic_timerCallback";
  callback_options.priority = 50;
  callback_options.deadline.sec = 1;
  callback_options.deadline.nsec = 0;
  // Component Timer - sporadic_timer
  timer_options = 
    rosmod::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Timer::sporadic_timerCallback, this, _1),
     &this->compQueue,
     callback_options,
     true,
     false);
  this->sporadic_timer = nh.createTimer(timer_options);



  this->comp_sync_pub = nh.advertise<std_msgs::Bool>("component_synchronization", 1000);
  
  rosmod::SubscribeOptions comp_sync_sub_options;
  rosmod::ROSMOD_Callback_Options sync_callback_options;
  comp_sync_sub_options = rosmod::SubscribeOptions::create<std_msgs::Bool>
    ("component_synchronization",
     1000,
     boost::bind(&Timer::component_synchronization_OnOneData, this, _1),
     rosmod::VoidPtr(),
     &this->compQueue,
     sync_callback_options);
  this->comp_sync_sub = nh.subscribe(comp_sync_sub_options);

  rosmod::Time now = rosmod::Time::now();
  while ( this->comp_sync_sub.getNumPublishers() < this->num_comps_to_sync &&
	  (rosmod::Time::now() - now) < rosmod::Duration(comp_sync_timeout) )
    ros::Duration(0.1).sleep();
  ros::Duration(0.5).sleep();
  this->comp_sync_sub.shutdown();
  this->comp_sync_pub.shutdown();

  this->initOneShotTimer.start();
  this->periodic_timer.start();
  this->sporadic_timer.start();
  
  compQueue.ROSMOD_LOGGER.CREATE_FILE(pwd + "ROSMOD_DEBUG." + nodeName + "." + compName + ".log");
  compQueue.ROSMOD_LOGGER.SET_PERIODICITY(is_periodic_logging);
  compQueue.ROSMOD_LOGGER.CHANGE_LOG_SIZE(periodic_log_unit);
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new Timer(config,argc,argv);
  }
}
