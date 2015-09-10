#include "dsc/controller_1.hpp"


//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void controller_1::Init(const ros::TimerEvent& event)
{
  // Initialize Here
  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker



// Subscriber Callback - ryg_state_sub
//# Start ryg_state_sub_OnOneData Marker
void controller_1::ryg_state_sub_OnOneData(const dsc::ryg_state::ConstPtr& received_data)
{
  // Business Logic for ryg_state_sub Subscriber

  
}
//# End ryg_state_sub_OnOneData Marker
// Subscriber Callback - l1_ew_in
//# Start l1_ew_in_OnOneData Marker
void controller_1::l1_ew_in_OnOneData(const dsc::sensor_state::ConstPtr& received_data)
{
  // Business Logic for l1_ew_in Subscriber

  
}
//# End l1_ew_in_OnOneData Marker
// Subscriber Callback - l1_ew_out
//# Start l1_ew_out_OnOneData Marker
void controller_1::l1_ew_out_OnOneData(const dsc::sensor_state::ConstPtr& received_data)
{
  // Business Logic for l1_ew_out Subscriber

  
}
//# End l1_ew_out_OnOneData Marker
// Subscriber Callback - l2_ew_in
//# Start l2_ew_in_OnOneData Marker
void controller_1::l2_ew_in_OnOneData(const dsc::sensor_state::ConstPtr& received_data)
{
  // Business Logic for l2_ew_in Subscriber

  
}
//# End l2_ew_in_OnOneData Marker
// Subscriber Callback - l2_ew_out
//# Start l2_ew_out_OnOneData Marker
void controller_1::l2_ew_out_OnOneData(const dsc::sensor_state::ConstPtr& received_data)
{
  // Business Logic for l2_ew_out Subscriber

  
}
//# End l2_ew_out_OnOneData Marker
// Subscriber Callback - l1_ns_in
//# Start l1_ns_in_OnOneData Marker
void controller_1::l1_ns_in_OnOneData(const dsc::sensor_state::ConstPtr& received_data)
{
  // Business Logic for l1_ns_in Subscriber

  
}
//# End l1_ns_in_OnOneData Marker
// Subscriber Callback - l1_ns_out
//# Start l1_ns_out_OnOneData Marker
void controller_1::l1_ns_out_OnOneData(const dsc::sensor_state::ConstPtr& received_data)
{
  // Business Logic for l1_ns_out Subscriber

  
}
//# End l1_ns_out_OnOneData Marker
// Subscriber Callback - l2_ns_in
//# Start l2_ns_in_OnOneData Marker
void controller_1::l2_ns_in_OnOneData(const dsc::sensor_state::ConstPtr& received_data)
{
  // Business Logic for l2_ns_in Subscriber

  
}
//# End l2_ns_in_OnOneData Marker
// Subscriber Callback - l2_ns_out
//# Start l2_ns_out_OnOneData Marker
void controller_1::l2_ns_out_OnOneData(const dsc::sensor_state::ConstPtr& received_data)
{
  // Business Logic for l2_ns_out Subscriber

  
}
//# End l2_ns_out_OnOneData Marker

// Timer Callback - controller_timer
//# Start controller_timerCallback Marker
void controller_1::controller_timerCallback(const ros::TimerEvent& event)
{
  // Business Logic for controller_timer Timer

}
//# End controller_timerCallback Marker


// Destructor - Cleanup Ports & Timers
controller_1::~controller_1()
{
  controller_timer.stop();
  ryg_control_pub.shutdown();
  ryg_state_sub.shutdown();
  l1_ew_in.shutdown();
  l1_ew_out.shutdown();
  l2_ew_in.shutdown();
  l2_ew_out.shutdown();
  l1_ns_in.shutdown();
  l1_ns_out.shutdown();
  l2_ns_in.shutdown();
  l2_ns_out.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void controller_1::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Scheduling Scheme is FIFO

  // Component Subscriber - ryg_state_sub
  advertiseName = "ryg_state";
  if (portGroupMap.find("ryg_state_sub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["ryg_state_sub"];
  ros::SubscribeOptions ryg_state_sub_options;
  ryg_state_sub_options = ros::SubscribeOptions::create<dsc::ryg_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller_1::ryg_state_sub_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->ryg_state_sub = nh.subscribe(ryg_state_sub_options);  
  // Component Subscriber - l1_ew_in
  advertiseName = "sensor_state";
  if (portGroupMap.find("l1_ew_in") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["l1_ew_in"];
  ros::SubscribeOptions l1_ew_in_options;
  l1_ew_in_options = ros::SubscribeOptions::create<dsc::sensor_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller_1::l1_ew_in_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->l1_ew_in = nh.subscribe(l1_ew_in_options);  
  // Component Subscriber - l1_ew_out
  advertiseName = "sensor_state";
  if (portGroupMap.find("l1_ew_out") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["l1_ew_out"];
  ros::SubscribeOptions l1_ew_out_options;
  l1_ew_out_options = ros::SubscribeOptions::create<dsc::sensor_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller_1::l1_ew_out_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->l1_ew_out = nh.subscribe(l1_ew_out_options);  
  // Component Subscriber - l2_ew_in
  advertiseName = "sensor_state";
  if (portGroupMap.find("l2_ew_in") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["l2_ew_in"];
  ros::SubscribeOptions l2_ew_in_options;
  l2_ew_in_options = ros::SubscribeOptions::create<dsc::sensor_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller_1::l2_ew_in_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->l2_ew_in = nh.subscribe(l2_ew_in_options);  
  // Component Subscriber - l2_ew_out
  advertiseName = "sensor_state";
  if (portGroupMap.find("l2_ew_out") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["l2_ew_out"];
  ros::SubscribeOptions l2_ew_out_options;
  l2_ew_out_options = ros::SubscribeOptions::create<dsc::sensor_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller_1::l2_ew_out_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->l2_ew_out = nh.subscribe(l2_ew_out_options);  
  // Component Subscriber - l1_ns_in
  advertiseName = "sensor_state";
  if (portGroupMap.find("l1_ns_in") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["l1_ns_in"];
  ros::SubscribeOptions l1_ns_in_options;
  l1_ns_in_options = ros::SubscribeOptions::create<dsc::sensor_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller_1::l1_ns_in_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->l1_ns_in = nh.subscribe(l1_ns_in_options);  
  // Component Subscriber - l1_ns_out
  advertiseName = "sensor_state";
  if (portGroupMap.find("l1_ns_out") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["l1_ns_out"];
  ros::SubscribeOptions l1_ns_out_options;
  l1_ns_out_options = ros::SubscribeOptions::create<dsc::sensor_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller_1::l1_ns_out_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->l1_ns_out = nh.subscribe(l1_ns_out_options);  
  // Component Subscriber - l2_ns_in
  advertiseName = "sensor_state";
  if (portGroupMap.find("l2_ns_in") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["l2_ns_in"];
  ros::SubscribeOptions l2_ns_in_options;
  l2_ns_in_options = ros::SubscribeOptions::create<dsc::sensor_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller_1::l2_ns_in_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->l2_ns_in = nh.subscribe(l2_ns_in_options);  
  // Component Subscriber - l2_ns_out
  advertiseName = "sensor_state";
  if (portGroupMap.find("l2_ns_out") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["l2_ns_out"];
  ros::SubscribeOptions l2_ns_out_options;
  l2_ns_out_options = ros::SubscribeOptions::create<dsc::sensor_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&controller_1::l2_ns_out_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->l2_ns_out = nh.subscribe(l2_ns_out_options);  

  // Component Publisher - ryg_control_pub
  advertiseName = "ryg_control";
  if (portGroupMap.find("ryg_control_pub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["ryg_control_pub"];
  this->ryg_control_pub = nh.advertise<dsc::ryg_control>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&controller_1::Init, this, _1),
     &this->compQueue,
     true,
     false);
  this->initOneShotTimer = nh.createTimer(timer_options);
  this->initOneShotTimer.stop();
  // Component Timer - controller_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.1),
     boost::bind(&controller_1::controller_timerCallback, this, _1),
     &this->compQueue, false, false);
  this->controller_timer = nh.createTimer(timer_options);

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


  this->comp_sync_pub = nh.advertise<std_msgs::Bool>("component_synchronization", 1000);
  
  ros::SubscribeOptions comp_sync_sub_options;
  comp_sync_sub_options = ros::SubscribeOptions::create<std_msgs::Bool>
    ("component_synchronization",
     1000,
     boost::bind(&controller_1::component_synchronization_OnOneData, this, _1),
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
  this->controller_timer.start();
  
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new controller_1(config,argc,argv);
  }
}
