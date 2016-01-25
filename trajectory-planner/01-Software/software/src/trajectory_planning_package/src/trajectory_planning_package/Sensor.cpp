#include "trajectory_planning_package/Sensor.hpp"

//# Start User Globals Marker
#include <math.h>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/random/mersenne_twister.hpp>
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Sensor::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering Sensor::init_timer_operation");
#endif
  // Initialize Here
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting Sensor::init_timer_operation");
#endif  
}
//# End Init Marker


// Server Operation - trajectory_server
//# Start compute_operation Marker
#pragma optimize( "", off )
bool Sensor::compute_operation(trajectory_planning_package::compute::Request  &req,
  trajectory_planning_package::compute::Response &res)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering Sensor::compute_operation");
#endif

  boost::random::mt19937 rng;
  boost::random::uniform_int_distribution<> loop_iteration_random(5600000 * 0.6, 5600000);
  int loop_max = loop_iteration_random(rng);  
  
  // Business Logic for trajectory_server_operation
  for(int i=0; i < loop_max; i++) {
    double result = 0.0;
    double x = 41865185131.214415;
    double y = 562056205.1515;
    result = x*y;
  }  
  logger->log("INFO", "Sensor::Server::Computing new result");
  float x_sq = pow(req.x, 2);
  float y_sq = pow(req.y, 2);
  float z_sq = pow(req.z, 2);
  float mean_sq = (x_sq + y_sq + z_sq)/3.0;
  res.result = sqrt(mean_sq);
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting Sensor::compute_operation");
#endif
  return true;
}
#pragma optimize( "", on )
//# End compute_operation Marker

// Timer Callback - sensor_timer
//# Start sensor_timer_operation Marker
#pragma optimize( "", off )
void Sensor::sensor_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering Sensor::sensor_timer_operation");
#endif
  // Business Logic for sensor_timer_operation
  for(int i=0; i < 2800000; i++) {
    double result = 0.0;
    double x = 41865185131.214415;
    double y = 562056205.1515;
    result = x*y;
  }  
  trajectory_planning_package::sensor_reading reading;
  reading.x = 5.3452;
  reading.y = 7.5209;
  reading.z = 9.1525;
  reading.state = "SENSING";
  state_publisher.publish(reading);
  logger->log("INFO", "Sensor::Timer::Publishing current sensor state");
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting Sensor::sensor_timer_operation");
#endif
}
#pragma optimize( "", on )
//# End sensor_timer_operation Marker


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
  // Component Publisher - state_publisher
  advertiseName = "sensor_reading";
  if (config.portGroupMap.find("state_publisher") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["state_publisher"];
  this->state_publisher = nh.advertise<trajectory_planning_package::sensor_reading>(advertiseName.c_str(), 1000);

  // Configure all provided services associated with this component
#ifdef USE_ROSMOD  
  callback_options.alias = "compute_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec =0;
  callback_options.deadline.nsec = 600000000;
#endif    
  // Component Server - trajectory_server
  advertiseName = "compute";
  if (config.portGroupMap.find("trajectory_server") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["trajectory_server"];
  NAMESPACE::AdvertiseServiceOptions trajectory_server_server_options;
  trajectory_server_server_options = NAMESPACE::AdvertiseServiceOptions::create<trajectory_planning_package::compute>
      (advertiseName.c_str(),
       boost::bind(&Sensor::compute_operation, this, _1, _2),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD       
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif
  this->trajectory_server = nh.advertiseService(trajectory_server_server_options);

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
     boost::bind(&Sensor::component_sync_operation, this, _1),
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
     boost::bind(&Sensor::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();
#ifdef USE_ROSMOD   
  callback_options.alias = "sensor_timer_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 400000000;
#endif
  // Component Timer - sensor_timer
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(1.0),
     boost::bind(&Sensor::sensor_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->sensor_timer = nh.createTimer(timer_options);


  this->init_timer.start();
  this->sensor_timer.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new Sensor(config,argc,argv);
  }
}
