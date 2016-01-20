#include "uav_package/Main_Controller.hpp"

//# Start User Globals Marker
#include "unistd.h"
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Main_Controller::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering Main_Controller::init_timer_operation");
#endif
  // Initialize Here
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting Main_Controller::init_timer_operation");
#endif  
}
//# End Init Marker



// Subscriber Operation - sensor_state_subscriber
//# Start sensor_state_subscriber_operation Marker
#pragma optimize( "", off )
void Main_Controller::sensor_state_subscriber_operation(const uav_package::sensor_state::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering Main_Controller::sensor_state_subscriber_operation");
#endif
  // Business Logic for sensor_state_subscriber_operation
  logger->log("DEBUG", "Received new sensor data! [%d, %d, %d, %f, %f, %f, %f, %f]", 
	      received_data->accX, received_data->accY, received_data->accZ, 
	      received_data->latitude, received_data->longitude, 
	      received_data->speed, received_data->altitude); 

  unsigned int compute_time_us = 46000;
  for(int i=0; i < 600000; i++) {
    double result = 0.0;
    double x = 41865185131.214415;
    double y = 562056205.1515;
    result = x*y;
  }  
  uav_package::control_command new_command;
  new_command.newHeading =  static_cast <float> (rand()) / 
    (static_cast <float> (RAND_MAX/(360.0)));
  new_command.newAltitude = 1.8 + static_cast <float> (rand()) / 
    (static_cast <float> (RAND_MAX/(9.4-1.8))); 
  int new_state = rand() % 4;
  new_command.command = "State" + std::to_string(new_state);
  control_command_publisher.publish(new_command);
  for(int i=0; i < 25000; i++) {
    double result = 0.0;
    double x = 41865185131.214415;
    double y = 562056205.1515;
    result = x*y;
  }
  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting Main_Controller::sensor_state_subscriber_operation");
#endif
}
#pragma optimize( "", on )
//# End sensor_state_subscriber_operation Marker


// Destructor - Cleanup Ports & Timers
Main_Controller::~Main_Controller()
{
  control_command_publisher.shutdown();
  sensor_state_subscriber.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Main_Controller::startUp()
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
  // Configure all publishers associated with this component
  // Component Publisher - control_command_publisher
  advertiseName = "control_command";
  if (config.portGroupMap.find("control_command_publisher") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["control_command_publisher"];
  this->control_command_publisher = nh.advertise<uav_package::control_command>(advertiseName.c_str(), 1000);


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
     boost::bind(&Main_Controller::component_sync_operation, this, _1),
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
  callback_options.alias = "sensor_state_subscriber_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 50000000;
#endif  
  // Component Subscriber - sensor_state_subscriber
  advertiseName = "sensor_state";
  if (config.portGroupMap.find("sensor_state_subscriber") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["sensor_state_subscriber"];
  NAMESPACE::SubscribeOptions sensor_state_subscriber_options;
  sensor_state_subscriber_options = NAMESPACE::SubscribeOptions::create<uav_package::sensor_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&Main_Controller::sensor_state_subscriber_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  this->sensor_state_subscriber = nh.subscribe(sensor_state_subscriber_options);

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
     boost::bind(&Main_Controller::init_timer_operation, this, _1),
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
    return new Main_Controller(config,argc,argv);
  }
}
