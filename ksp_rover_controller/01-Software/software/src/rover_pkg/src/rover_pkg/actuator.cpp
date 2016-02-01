#include "rover_pkg/actuator.hpp"

KRPCI krpci_client;
//# Start User Globals Marker
uint64_t vesselID;
uint64_t controlID;
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void actuator::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering actuator::init_timer_operation");
#endif
  // Initialize Here
  std::string ksp_host;
  int ksp_port;
  for (int i=0; i<node_argc; i++)
    {
      if (!strcmp(node_argv[i], "--ksp_host"))
	{
	  ksp_host = node_argv[i+1];
	}
      if (!strcmp(node_argv[i], "--ksp_port"))
	{
	  ksp_port = atoi(node_argv[i+1]);
	}
    }
  krpci_client.SetPort(ksp_port);
  krpci_client.SetIP(ksp_host);
  if (krpci_client.Connect()) {
    krpci_client.get_ActiveVessel(vesselID);
    krpci_client.Vessel_get_Control(vesselID, controlID);
  }  
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting actuator::init_timer_operation");
#endif  
}
//# End Init Marker



// Subscriber Operation - control_command_sub
//# Start control_command_sub_operation Marker
void actuator::control_command_sub_operation(const rover_pkg::control_command::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering actuator::control_command_sub_operation");
#endif
  // Business Logic for control_command_sub_operation
  float wheel_throttle = received_data->new_wheel_throttle;
  float wheel_steering = received_data->new_wheel_steering;
  krpci_client.Control_set_WheelThrottle(controlID, wheel_throttle);
  krpci_client.Control_set_WheelSteering(controlID, wheel_steering);
  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting actuator::control_command_sub_operation");
#endif
}
//# End control_command_sub_operation Marker


// Destructor - Cleanup Ports & Timers
actuator::~actuator()
{
  control_command_sub.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void actuator::startUp()
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
     boost::bind(&actuator::component_sync_operation, this, _1),
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
  callback_options.alias = "control_command_sub_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif  
  // Component Subscriber - control_command_sub
  advertiseName = "control_command";
  if (config.portGroupMap.find("control_command_sub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["control_command_sub"];
  NAMESPACE::SubscribeOptions control_command_sub_options;
  control_command_sub_options = NAMESPACE::SubscribeOptions::create<rover_pkg::control_command>
      (advertiseName.c_str(),
       1000,
       boost::bind(&actuator::control_command_sub_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  this->control_command_sub = nh.subscribe(control_command_sub_options);

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
     boost::bind(&actuator::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();

  krpci_client.SetName(config.nodeName + "_" + config.compName);

  this->init_timer.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new actuator(config,argc,argv);
  }
}
