#include "uav_package/Servo_Actuator.hpp"

//# Start User Globals Marker
#include "unistd.h"
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Servo_Actuator::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering Servo_Actuator::init_timer_operation");
#endif
  // Initialize Here
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting Servo_Actuator::init_timer_operation");
#endif  
}
//# End Init Marker



// Subscriber Operation - control_command_subscriber
//# Start control_command_subscriber_operation Marker
void Servo_Actuator::control_command_subscriber_operation(const uav_package::control_command::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering Servo_Actuator::control_command_subscriber_operation");
#endif
  // Business Logic for control_command_subscriber_operation
  logger->log("DEBUG", "Received new command! [%f %f]", received_data->newHeading,
	      received_data->newAltitude);
  usleep(6000);
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting Servo_Actuator::control_command_subscriber_operation");
#endif
}
//# End control_command_subscriber_operation Marker


// Destructor - Cleanup Ports & Timers
Servo_Actuator::~Servo_Actuator()
{
  control_command_subscriber.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Servo_Actuator::startUp()
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
     boost::bind(&Servo_Actuator::component_sync_operation, this, _1),
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
  callback_options.alias = "control_command_subscriber_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 15000000;
#endif  
  // Component Subscriber - control_command_subscriber
  advertiseName = "control_command";
  if (config.portGroupMap.find("control_command_subscriber") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["control_command_subscriber"];
  NAMESPACE::SubscribeOptions control_command_subscriber_options;
  control_command_subscriber_options = NAMESPACE::SubscribeOptions::create<uav_package::control_command>
      (advertiseName.c_str(),
       1000,
       boost::bind(&Servo_Actuator::control_command_subscriber_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  this->control_command_subscriber = nh.subscribe(control_command_subscriber_options);

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
     boost::bind(&Servo_Actuator::init_timer_operation, this, _1),
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
    return new Servo_Actuator(config,argc,argv);
  }
}
