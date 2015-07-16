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
  compQueue.ROSMOD_LOGGER.DEBUG("Entering periodic_timerCallback");
  // Business Logic for periodic_timer Timer

  compQueue.ROSMOD_LOGGER.DEBUG("Exiting periodic_timerCallback");
}
//# End periodic_timerCallback Marker
// Timer Callback - sporadic_timer
//# Start sporadic_timerCallback Marker
void Timer::sporadic_timerCallback(const rosmod::TimerEvent& event)
{
  compQueue.ROSMOD_LOGGER.DEBUG("Entering sporadic_timerCallback");
  // Business Logic for sporadic_timer Timer

  compQueue.ROSMOD_LOGGER.DEBUG("Exiting sporadic_timerCallback");
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

  // Scheduling Scheme is FIFO
  this->compQueue.scheduling_scheme = scheduling_scheme;
    
  rosmod::ROSMOD_Callback_Options callback_options;
  callback_options.alias = "Init_Timer";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0.0;
  callback_options.deadline.nsec = 40000;
  
  // Init Timer
  rosmod::TimerOptions timer_options;
  timer_options = 
    rosmod::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Timer::Init, this, _1),
     &this->compQueue,
     callback_options,
     true, true);
  this->initOneShotTimer = nh.createTimer(timer_options);

  callback_options.alias = "Periodic_Timer";
  
  // Component Timer - periodic_timer
  timer_options = 
    rosmod::TimerOptions
    (ros::Duration(1.0),
     boost::bind(&Timer::periodic_timerCallback, this, _1),
     &this->compQueue,
     callback_options,
     false,
     true);
  this->periodic_timer = nh.createTimer(timer_options);

  callback_options.alias = "Sporadic_Timer";
    
  // Component Timer - sporadic_timer
  timer_options = 
    rosmod::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Timer::sporadic_timerCallback, this, _1),
     &this->compQueue,
     callback_options,
     true, true);
  this->sporadic_timer = nh.createTimer(timer_options);

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

  compQueue.ROSMOD_LOGGER.CREATE_FILE(pwd + "ROSMOD_DEBUG." + nodeName + "." + compName + ".log");
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new Timer(config,argc,argv);
  }
}
