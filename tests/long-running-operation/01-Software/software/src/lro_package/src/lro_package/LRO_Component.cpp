#include "lro_package/LRO_Component.hpp"

//# Start User Globals Marker
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <unistd.h>
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void LRO_Component::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering LRO_Component::init_timer_operation");
#endif
  // Initialize Here
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting LRO_Component::init_timer_operation");
#endif  
}
//# End Init Marker



// Subscriber Operation - critical_subscriber_port
//# Start critical_subscriber_port_operation Marker
void LRO_Component::critical_subscriber_port_operation(const lro_package::Message::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering LRO_Component::critical_subscriber_port_operation");
#endif
  // Business Logic for critical_subscriber_port_operation
  boost::random::mt19937 rng;
  boost::random::uniform_int_distribution<> loop_iteration_random(280000000 * 0.6, 280000000);
  int loop_max = loop_iteration_random(rng);  
  
  // Business Logic for subscriber_port_operation
  for(int i=0; i < loop_max; i++) {
    double result = 0.0;
    double x = 41865185131.214415;
    double y = 562056205.1515;
    result = x*y;
  } 
  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting LRO_Component::critical_subscriber_port_operation");
#endif
}
//# End critical_subscriber_port_operation Marker

// Timer Callback - long_running_operation
//# Start long_running_operation_operation Marker
void LRO_Component::long_running_operation_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering LRO_Component::long_running_operation_operation");
#endif
  // Business Logic for long_running_operation_operation
  boost::random::mt19937 rng;
  boost::random::uniform_int_distribution<> loop_iteration_random(500000000 * 0.6, 500000000);
  int loop_max = loop_iteration_random(rng);
  for(int j=0; j < loop_max; j++) {
    usleep(2000000);
    for(long long int i=0; i < loop_max * 100; i++) {
      double result = 0.0;
      double x = 41865185131.214415;
      double y = 562056205.1515;
      result = x*y;
    }
    for(long long int i=0; i < loop_max * 100; i++) {
      double result = 0.0;
      double x = 41865185131.214415;
      double y = 562056205.1515;
      result = x*y;
    }    
    int waitingCallbackPriority = comp_queue.getWaitingCallbackPriority();
    int lro_timer_priority = 50;
    if (waitingCallbackPriority > lro_timer_priority)
      break;
  }

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting LRO_Component::long_running_operation_operation");
#endif
}
//# End long_running_operation_operation Marker


// Destructor - Cleanup Ports & Timers
LRO_Component::~LRO_Component()
{
  long_running_operation.stop();
  critical_subscriber_port.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void LRO_Component::startUp()
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
     boost::bind(&LRO_Component::component_sync_operation, this, _1),
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
  callback_options.alias = "critical_subscriber_port_operation";
  callback_options.priority = 75;
  callback_options.deadline.sec = 1;
  callback_options.deadline.nsec = 0;
#endif  
  // Component Subscriber - critical_subscriber_port
  advertiseName = "Message";
  if (config.portGroupMap.find("critical_subscriber_port") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["critical_subscriber_port"];
  NAMESPACE::SubscribeOptions critical_subscriber_port_options;
  critical_subscriber_port_options = NAMESPACE::SubscribeOptions::create<lro_package::Message>
      (advertiseName.c_str(),
       1000,
       boost::bind(&LRO_Component::critical_subscriber_port_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  this->critical_subscriber_port = nh.subscribe(critical_subscriber_port_options);

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
     boost::bind(&LRO_Component::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();
#ifdef USE_ROSMOD   
  callback_options.alias = "long_running_operation_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 5;
  callback_options.deadline.nsec = 0;
#endif
  // Component Timer - long_running_operation
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(1.0),
     boost::bind(&LRO_Component::long_running_operation_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->long_running_operation = nh.createTimer(timer_options);


  this->init_timer.start();
  this->long_running_operation.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new LRO_Component(config,argc,argv);
  }
}
