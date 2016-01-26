#include "periodic_timers/All_Timers.hpp"

//# Start User Globals Marker
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/random/mersenne_twister.hpp>
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void All_Timers::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering All_Timers::init_timer_operation");
#endif
  // Initialize Here
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting All_Timers::init_timer_operation");
#endif  
}
//# End Init Marker


// Timer Callback - Timer_1
//# Start Timer_1_operation Marker
#pragma optimize( "", off )
void All_Timers::Timer_1_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering All_Timers::Timer_1_operation");
#endif
  // Business Logic for Timer_1_operation
  boost::random::mt19937 rng;
  boost::random::uniform_int_distribution<> loop_iteration_random(1500000 * 0.6, 1500000);
  int loop_max = loop_iteration_random(rng);  
  
  // Business Logic for Timer_1_operation
  for(int i=0; i < loop_max; i++) {
    double result = 0.0;
    double x = 41865185131.214415;
    double y = 562056205.1515;
    result = x*y;
  }
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting All_Timers::Timer_1_operation");
#endif
}
#pragma optimize( "", on )
//# End Timer_1_operation Marker
// Timer Callback - Timer_2
//# Start Timer_2_operation Marker
#pragma optimize( "", off )
void All_Timers::Timer_2_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering All_Timers::Timer_2_operation");
#endif
  // Business Logic for Timer_2_operation
  boost::random::mt19937 rng;
  boost::random::uniform_int_distribution<> loop_iteration_random(750000 * 0.6, 750000);
  int loop_max = loop_iteration_random(rng);  
  
  // Business Logic for Timer_2_operation
  for(int i=0; i < loop_max; i++) {
    double result = 0.0;
    double x = 41865185131.214415;
    double y = 562056205.1515;
    result = x*y;
  }
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting All_Timers::Timer_2_operation");
#endif
}
#pragma optimize( "", on )
//# End Timer_2_operation Marker
// Timer Callback - Timer_3
//# Start Timer_3_operation Marker
#pragma optimize( "", off )
void All_Timers::Timer_3_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering All_Timers::Timer_3_operation");
#endif
  // Business Logic for Timer_3_operation
  boost::random::mt19937 rng;
  boost::random::uniform_int_distribution<> loop_iteration_random(420000 * 0.6, 420000);
  int loop_max = loop_iteration_random(rng);  
  
  // Business Logic for Timer_3_operation
  for(int i=0; i < loop_max; i++) {
    double result = 0.0;
    double x = 41865185131.214415;
    double y = 562056205.1515;
    result = x*y;
  }
  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting All_Timers::Timer_3_operation");
#endif
}
#pragma optimize( "", on )
//# End Timer_3_operation Marker
// Timer Callback - Timer_4
//# Start Timer_4_operation Marker
#pragma optimize( "", off )
void All_Timers::Timer_4_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering All_Timers::Timer_4_operation");
#endif
  // Business Logic for Timer_4_operation
  boost::random::mt19937 rng;
  boost::random::uniform_int_distribution<> loop_iteration_random(280000 * 0.6, 280000);
  int loop_max = loop_iteration_random(rng);  
  
  // Business Logic for Timer_4_operation
  for(int i=0; i < loop_max; i++) {
    double result = 0.0;
    double x = 41865185131.214415;
    double y = 562056205.1515;
    result = x*y;
  }
  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting All_Timers::Timer_4_operation");
#endif
}
#pragma optimize( "", on )
//# End Timer_4_operation Marker
// Timer Callback - Timer_5
//# Start Timer_5_operation Marker
#pragma optimize( "", off )
void All_Timers::Timer_5_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering All_Timers::Timer_5_operation");
#endif
  // Business Logic for Timer_5_operation
  boost::random::mt19937 rng;
  boost::random::uniform_int_distribution<> loop_iteration_random(50000 * 0.6, 50000);
  int loop_max = loop_iteration_random(rng);
  
  // Business Logic for Timer_5_operation
  for(int i=0; i < loop_max; i++) {
    double result = 0.0;
    double x = 41865185131.214415;
    double y = 562056205.1515;
    result = x*y;
  }
  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting All_Timers::Timer_5_operation");
#endif
}
#pragma optimize( "", on )
//# End Timer_5_operation Marker


// Destructor - Cleanup Ports & Timers
All_Timers::~All_Timers()
{
  Timer_1.stop();
  Timer_2.stop();
  Timer_3.stop();
  Timer_4.stop();
  Timer_5.stop();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void All_Timers::startUp()
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
     boost::bind(&All_Timers::component_sync_operation, this, _1),
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
     boost::bind(&All_Timers::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();
#ifdef USE_ROSMOD   
  callback_options.alias = "Timer_1_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 300000000;
#endif
  // Component Timer - Timer_1
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(5.0),
     boost::bind(&All_Timers::Timer_1_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->Timer_1 = nh.createTimer(timer_options);
#ifdef USE_ROSMOD   
  callback_options.alias = "Timer_2_operation";
  callback_options.priority = 60;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 150000000;
#endif
  // Component Timer - Timer_2
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(2.5),
     boost::bind(&All_Timers::Timer_2_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->Timer_2 = nh.createTimer(timer_options);
#ifdef USE_ROSMOD   
  callback_options.alias = "Timer_3_operation";
  callback_options.priority = 70;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 75000000;
#endif
  // Component Timer - Timer_3
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(1.0),
     boost::bind(&All_Timers::Timer_3_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->Timer_3 = nh.createTimer(timer_options);
#ifdef USE_ROSMOD   
  callback_options.alias = "Timer_4_operation";
  callback_options.priority = 75;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 37500000;
#endif
  // Component Timer - Timer_4
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(0.5),
     boost::bind(&All_Timers::Timer_4_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->Timer_4 = nh.createTimer(timer_options);
#ifdef USE_ROSMOD   
  callback_options.alias = "Timer_5_operation";
  callback_options.priority = 80;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 10000000;
#endif
  // Component Timer - Timer_5
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(0.25),
     boost::bind(&All_Timers::Timer_5_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->Timer_5 = nh.createTimer(timer_options);


  this->init_timer.start();
  this->Timer_1.start();
  this->Timer_2.start();
  this->Timer_3.start();
  this->Timer_4.start();
  this->Timer_5.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new All_Timers(config,argc,argv);
  }
}
