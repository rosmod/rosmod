#include "rover_pkg/low_level_controller.hpp"


//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void low_level_controller::Init(const ros::TimerEvent& event)
{
  LOGGER.DEBUG("Entering low_level_controller::Init");
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
  initOneShotTimer.stop();
  LOGGER.DEBUG("Exiting low_level_controller::Init");
}
//# End Init Marker



// Subscriber Callback - vessel_state_sub
//# Start vessel_state_sub_OnOneData Marker
void low_level_controller::vessel_state_sub_OnOneData(const rover_pkg::vessel_state::ConstPtr& received_data)
{
  LOGGER.DEBUG("Entering low_level_controller::vessel_state_sub_OnOneData");
  // Business Logic for vessel_state_sub Subscriber
  current_heading = received_data->heading;
  current_speed = received_data->speed;

  LOGGER.DEBUG("Exiting low_level_controller::vessel_state_sub_OnOneData");
}
//# End vessel_state_sub_OnOneData Marker
// Subscriber Callback - goal_state_sub
//# Start goal_state_sub_OnOneData Marker
void low_level_controller::goal_state_sub_OnOneData(const rover_pkg::goal_state::ConstPtr& received_data)
{
  LOGGER.DEBUG("Entering low_level_controller::goal_state_sub_OnOneData");
  // Business Logic for goal_state_sub Subscriber
  goal_heading = received_data->goal_heading;
  goal_speed = received_data->goal_speed;
  
  speed_pid.setPoint(goal_speed);
  heading_pid.setPoint(goal_heading);
  LOGGER.DEBUG("Exiting low_level_controller::goal_state_sub_OnOneData");
}
//# End goal_state_sub_OnOneData Marker

// Timer Callback - control_timer
//# Start control_timerCallback Marker
void low_level_controller::control_timerCallback(const ros::TimerEvent& event)
{
  LOGGER.DEBUG("Entering low_level_controller::control_timerCallback");
  // Business Logic for control_timer Timer
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

  LOGGER.DEBUG("CURRENT SENSOR HEADING AND SPEED: %f ;  %f",current_heading, current_speed);
  LOGGER.DEBUG("GOAL HEADING AND SPEED: %f ;  %f",goal_heading,goal_speed);
  LOGGER.DEBUG("ACTUATOR STEERING AND THROTTLE: %f ;  %f",new_wheel_steering,new_wheel_throttle);

  rover_pkg::control_command new_command;
  new_command.new_wheel_throttle = new_wheel_throttle;
  new_command.new_wheel_steering = new_wheel_steering;
  control_command_pub.publish(new_command);

  LOGGER.DEBUG("Exiting low_level_controller::control_timerCallback");
}
//# End control_timerCallback Marker


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
  ros::NodeHandle nh;
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

  // Component Subscriber - vessel_state_sub
  advertiseName = "vessel_state";
  if (portGroupMap.find("vessel_state_sub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["vessel_state_sub"];
  ros::SubscribeOptions vessel_state_sub_options;
  vessel_state_sub_options = ros::SubscribeOptions::create<rover_pkg::vessel_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&low_level_controller::vessel_state_sub_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->vessel_state_sub = nh.subscribe(vessel_state_sub_options);  
  // Component Subscriber - goal_state_sub
  advertiseName = "goal_state";
  if (portGroupMap.find("goal_state_sub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["goal_state_sub"];
  ros::SubscribeOptions goal_state_sub_options;
  goal_state_sub_options = ros::SubscribeOptions::create<rover_pkg::goal_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&low_level_controller::goal_state_sub_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->goal_state_sub = nh.subscribe(goal_state_sub_options);  

  // Component Publisher - control_command_pub
  advertiseName = "control_command";
  if (portGroupMap.find("control_command_pub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["control_command_pub"];
  this->control_command_pub = nh.advertise<rover_pkg::control_command>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&low_level_controller::Init, this, _1),
     &this->compQueue,
     true,
     false);
  this->initOneShotTimer = nh.createTimer(timer_options);
  this->initOneShotTimer.stop();
  // Component Timer - control_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.2),
     boost::bind(&low_level_controller::control_timerCallback, this, _1),
     &this->compQueue, false, false);
  this->control_timer = nh.createTimer(timer_options);



  this->comp_sync_pub = nh.advertise<std_msgs::Bool>("component_synchronization", 1000);
  
  ros::SubscribeOptions comp_sync_sub_options;
  comp_sync_sub_options = ros::SubscribeOptions::create<std_msgs::Bool>
    ("component_synchronization",
     1000,
     boost::bind(&low_level_controller::component_synchronization_OnOneData, this, _1),
     ros::VoidPtr(),
     &this->compQueue);
  this->comp_sync_sub = nh.subscribe(comp_sync_sub_options);

  ros::Time now = ros::Time::now();
  while ( this->comp_sync_sub.getNumPublishers() < this->num_comps_to_sync &&
	  (ros::Time::now() - now) < ros::Duration(comp_sync_timeout) )
    ros::Duration(0.1).sleep();
  ros::Duration(0.5).sleep();
  this->comp_sync_sub.shutdown();
  this->comp_sync_pub.shutdown();

  this->initOneShotTimer.start();
  this->control_timer.start();
  
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new low_level_controller(config,argc,argv);
  }
}
