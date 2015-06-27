#include "ksp_stearwing_controller/PID.hpp"


//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void PID::Init(const ros::TimerEvent& event)
{
  // Initialize Here

  double ma_kp=0.05, ma_ki=0.0, ma_kd=0.05, ma_imax=50, ma_imin=-50;
  double p_kp=0.5, p_ki=0.5, p_kd=0.05, p_imax=500, p_imin=-500;
  double r_kp=0.05, r_ki=0.0, r_kd=0.005, r_imax=500, r_imin=-500;
  double h_kp=0.05, h_ki=0.0, h_kd=0.005, h_imax=500, h_imin=-500;

  for (int i=0;i<node_argc;i++)
    {
      if (!strcmp(node_argv[i],"--ma_kp"))
	{
	  ma_kp = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--ma_ki"))
	{
	  ma_ki = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--ma_kd"))
	{
	  ma_kd = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--ma_imax"))
	{
	  ma_imax = atof(node_argv[i+1]);
	  ma_imin = -ma_imax;
	}
      if (!strcmp(node_argv[i],"--p_kp"))
	{
	  p_kp = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--p_ki"))
	{
	  p_ki = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--p_kd"))
	{
	  p_kd = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--p_imax"))
	{
	  p_imax = atof(node_argv[i+1]);
	  p_imin = -p_imax;
	}
      if (!strcmp(node_argv[i],"--r_kp"))
	{
	  r_kp = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--r_ki"))
	{
	  r_ki = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--r_kd"))
	{
	  r_kd = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--r_imax"))
	{
	  r_imax = atof(node_argv[i+1]);
	  r_imin = -r_imax;
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
	  h_imin = -h_imax;
	}

    }

  mean_altitude_pid.setKp(ma_kp);
  mean_altitude_pid.setKi(ma_ki);
  mean_altitude_pid.setKd(ma_kd);
  mean_altitude_pid.setIntegratorMax(ma_imax);
  mean_altitude_pid.setIntegratorMin(ma_imin);

  pitch_pid.setKp(p_kp);
  pitch_pid.setKi(p_ki);
  pitch_pid.setKd(p_kd);
  pitch_pid.setIntegratorMax(p_imax);
  pitch_pid.setIntegratorMin(p_imin);

  roll_pid.setKp(r_kp);
  roll_pid.setKi(r_ki);
  roll_pid.setKd(r_kd);
  roll_pid.setIntegratorMax(r_imax);
  roll_pid.setIntegratorMin(r_imin);
  //roll_pid.setKp(0.05);
  //roll_pid.setKi(0);
  //roll_pid.setKd(0.005);

  heading_pid.setKp(h_kp);
  heading_pid.setKi(h_ki);
  heading_pid.setKd(h_kd);
  heading_pid.setIntegratorMax(h_imax);
  heading_pid.setIntegratorMin(h_imin);

  speed_pid.setKp(0.1);
  speed_pid.setKi(0.001);
  speed_pid.setKd(0.05);
  speed_pid.setIntegratorMax(50);
  speed_pid.setIntegratorMin(-50);
  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Subscriber Callback - sensor_subscriber
//# Start sensor_subscriber_OnOneData Marker
void PID::sensor_subscriber_OnOneData(const ksp_stearwing_controller::Sensor_Reading::ConstPtr& received_data)
{
  // Business Logic for sensor_subscriber Subscriber
  current_throttle = received_data->throttle;
  current_pitch = received_data->pitch;
  current_roll = received_data->roll;
  current_heading = received_data->heading;
  current_mean_altitude = received_data->mean_altitude;
  current_speed = received_data->speed;
}
//# End sensor_subscriber_OnOneData Marker
// Subscriber Callback - pid_control_subscriber
//# Start pid_control_subscriber_OnOneData Marker
void PID::pid_control_subscriber_OnOneData(const ksp_stearwing_controller::Control_Command::ConstPtr& received_data)
{
  // Business Logic for pid_control_subscriber Subscriber
  goal_heading = received_data->goal_heading;
  double temp_altitude = goal_mean_altitude;
  if (received_data->goal_altitude != temp_altitude)
    goal_mean_altitude = received_data->goal_altitude;
  goal_speed = received_data->goal_speed;
  //  LOGGER.INFO("Control Subscriber::Heading=%f, Altitude=%f; Speed=%f", goal_heading, goal_mean_altitude, goal_speed);
  mean_altitude_pid.setPoint(goal_mean_altitude);

  if (received_data->goal_altitude != temp_altitude)
    {
      double gain = abs(current_mean_altitude-goal_mean_altitude) * 1/10000.0;
      //mean_altitude_pid.setKp(gain*5);
      //mean_altitude_pid.setKi(gain);
      //mean_altitude_pid.setKd(gain*500.0);
    }

  heading_pid.setPoint(goal_heading);
  speed_pid.setPoint(goal_speed);
}
//# End pid_control_subscriber_OnOneData Marker

// Timer Callback - pid_timer
//# Start pid_timerCallback Marker
void PID::pid_timerCallback(const ros::TimerEvent& event)
{
  // Business Logic for pid_timer Timer
  float new_pitch = 0;
  float new_roll = 0;
  float new_yaw = 0;
  float new_throttle = 0;
  float new_heading = 0;

  float new_AoA = 0;

  // NEED TO DO PID HERE
  new_AoA = mean_altitude_pid.update(current_mean_altitude);
  pitch_pid.setPoint(new_AoA);
  new_pitch = pitch_pid.update(current_pitch);
  new_heading = heading_pid.update(current_heading);  
  new_roll = roll_pid.update(-new_heading);
  LOGGER.INFO("Current Heading=%f, New Heading=%f; New Roll=%f", current_heading, new_heading, new_roll);
  //new_yaw = heading_pid.update(current_heading);
  new_throttle = speed_pid.update(current_speed);

  ksp_stearwing_controller::Actuation_Command new_actuation;
  new_actuation.new_pitch = new_pitch;
  new_actuation.new_roll = new_roll;
  new_actuation.new_yaw = new_yaw;
  new_actuation.new_throttle = new_throttle;
  actuator_control_publisher.publish(new_actuation);
}
//# End pid_timerCallback Marker


// Destructor - Cleanup Ports & Timers
PID::~PID()
{
  pid_timer.stop();
  actuator_control_publisher.shutdown();
  sensor_subscriber.shutdown();
  pid_control_subscriber.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void PID::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Subscriber - sensor_subscriber
  advertiseName = "Sensor_Reading";
  if (portGroupMap.find("sensor_subscriber") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["sensor_subscriber"];
  ros::SubscribeOptions sensor_subscriber_options;
  sensor_subscriber_options = ros::SubscribeOptions::create<ksp_stearwing_controller::Sensor_Reading>
      (advertiseName.c_str(),
       1000,
       boost::bind(&PID::sensor_subscriber_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->sensor_subscriber = nh.subscribe(sensor_subscriber_options);
  // Component Subscriber - pid_control_subscriber
  advertiseName = "Control_Command";
  if (portGroupMap.find("pid_control_subscriber") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["pid_control_subscriber"];
  ros::SubscribeOptions pid_control_subscriber_options;
  pid_control_subscriber_options = ros::SubscribeOptions::create<ksp_stearwing_controller::Control_Command>
      (advertiseName.c_str(),
       1000,
       boost::bind(&PID::pid_control_subscriber_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->pid_control_subscriber = nh.subscribe(pid_control_subscriber_options);

  // Component Publisher - actuator_control_publisher
  advertiseName = "Actuation_Command";
  if (portGroupMap.find("actuator_control_publisher") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["actuator_control_publisher"];
  this->actuator_control_publisher = nh.advertise<ksp_stearwing_controller::Actuation_Command>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&PID::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - pid_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.2),
     boost::bind(&PID::pid_timerCallback, this, _1),
     &this->compQueue);
  this->pid_timer = nh.createTimer(timer_options);

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

}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new PID(config,argc,argv);
  }
}
