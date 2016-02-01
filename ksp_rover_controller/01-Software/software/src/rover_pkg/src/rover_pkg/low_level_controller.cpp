#include "rover_pkg/low_level_controller.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void low_level_controller::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering low_level_controller::init_timer_operation");
#endif
  // Initialize Here
  double s_kp=0.1, s_ki=0.01, s_kd=0.00001, s_imax=100, s_imin=0;
  double h_kp=0.05, h_ki=0.0, h_kd=0.0, h_imax=100, h_imin=-100;

  for (int i=0;i<node_argc;i++)
    {
      if (!strcmp(node_argv[i],"--s_kp"))
	{
	  s_kp = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--s_ki"))
	{
	  s_ki = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--s_kd"))
	{
	  s_kd = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--s_imax"))
	{
	  s_imax = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--s_imin"))
	{
	  s_imin = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--h_kp"))
	{
	  h_kp = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--h_ki"))
	{
	  h_ki = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--h_kd"))
	{
	  h_kd = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--h_imax"))
	{
	  h_imax = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--h_imin"))
	{
	  h_imin = atof(node_argv[i+1]);
	}
    }

  heading_pid.setKp(h_kp);
  heading_pid.setKi(h_ki);
  heading_pid.setKd(h_kd);
  heading_pid.setIntegratorMax(h_imax);
  heading_pid.setIntegratorMin(h_imin);
  
  speed_pid.setKp(s_kp);
  speed_pid.setKi(s_ki);
  speed_pid.setKd(s_kd);
  speed_pid.setIntegratorMax(s_imax);
  speed_pid.setIntegratorMin(s_imin);
  
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting low_level_controller::init_timer_operation");
#endif  
}
//# End Init Marker

// Subscriber Operation - vessel_state_sub
//# Start vessel_state_sub_operation Marker
void low_level_controller::vessel_state_sub_operation(const rover_pkg::vessel_state::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering low_level_controller::vessel_state_sub_operation");
#endif
  // Business Logic for vessel_state_sub_operation
  current_heading = received_data->heading;
  current_speed = received_data->speed;
  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting low_level_controller::vessel_state_sub_operation");
#endif
}
//# End vessel_state_sub_operation Marker
// Subscriber Operation - goal_state_sub
//# Start goal_state_sub_operation Marker
void low_level_controller::goal_state_sub_operation(const rover_pkg::goal_state::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering low_level_controller::goal_state_sub_operation");
#endif
  // Business Logic for goal_state_sub_operation
  goal_heading = received_data->goal_heading;
  goal_speed = received_data->goal_speed;
  
  speed_pid.setPoint(goal_speed);
  heading_pid.setPoint(goal_heading);
  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting low_level_controller::goal_state_sub_operation");
#endif
}
//# End goal_state_sub_operation Marker

// Timer Callback - control_timer
//# Start control_timer_operation Marker
void low_level_controller::control_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering low_level_controller::control_timer_operation");
#endif
  // Business Logic for control_timer_operation
  float new_wheel_throttle = 0;
  float new_wheel_steering = 0;

  // NEED TO DO PID HERE
  float dh1 = abs(goal_heading - current_heading);
  float dh2 = 360-dh1;
  if (dh2 < dh1)
    {
      if ( goal_heading < current_heading )
	current_heading = current_heading - 360.0;
      else if ( current_heading < goal_heading )
	current_heading += 360.0;
    }
    
  new_wheel_steering = heading_pid.update(current_heading);  

  new_wheel_throttle = speed_pid.update(current_speed);

  rover_pkg::control_command new_command;
  new_command.new_wheel_throttle = new_wheel_throttle;
  new_command.new_wheel_steering = new_wheel_steering;
  control_command_pub.publish(new_command);

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting low_level_controller::control_timer_operation");
#endif
}
//# End control_timer_operation Marker


// Destructor - Cleanup Ports & Timers
low_level_controller::~low_level_controller()
{
  control_timer.stop();
  control_command_pub.shutdown();
  vessel_state_sub.shutdown();
  goal_state_sub.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void low_level_controller::startUp()
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
  // Component Publisher - control_command_pub
  advertiseName = "control_command";
  if (config.portGroupMap.find("control_command_pub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["control_command_pub"];
  this->control_command_pub = nh.advertise<rover_pkg::control_command>(advertiseName.c_str(), 1000);


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
     boost::bind(&low_level_controller::component_sync_operation, this, _1),
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
  callback_options.alias = "vessel_state_sub_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif  
  // Component Subscriber - vessel_state_sub
  advertiseName = "vessel_state";
  if (config.portGroupMap.find("vessel_state_sub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["vessel_state_sub"];
  NAMESPACE::SubscribeOptions vessel_state_sub_options;
  vessel_state_sub_options = NAMESPACE::SubscribeOptions::create<rover_pkg::vessel_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&low_level_controller::vessel_state_sub_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  this->vessel_state_sub = nh.subscribe(vessel_state_sub_options);
#ifdef USE_ROSMOD 
  callback_options.alias = "goal_state_sub_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif  
  // Component Subscriber - goal_state_sub
  advertiseName = "goal_state";
  if (config.portGroupMap.find("goal_state_sub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["goal_state_sub"];
  NAMESPACE::SubscribeOptions goal_state_sub_options;
  goal_state_sub_options = NAMESPACE::SubscribeOptions::create<rover_pkg::goal_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&low_level_controller::goal_state_sub_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  this->goal_state_sub = nh.subscribe(goal_state_sub_options);

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
     boost::bind(&low_level_controller::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();
#ifdef USE_ROSMOD   
  callback_options.alias = "control_timer_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif
  // Component Timer - control_timer
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(0.2),
     boost::bind(&low_level_controller::control_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->control_timer = nh.createTimer(timer_options);


  this->init_timer.start();
  this->control_timer.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new low_level_controller(config,argc,argv);
  }
}
