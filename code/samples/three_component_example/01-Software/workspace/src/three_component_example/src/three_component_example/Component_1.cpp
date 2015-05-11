#include "three_component_example/Component_1.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Component_1::Init(const ros::TimerEvent& event)
{
  // Initialize Here
  three_component_example::ComponentName compName;
  compName.name = "Component_1";
  LOGGER.INFO("Component_1::Name_Publisher::Publishing Component Name::%s", compName.name.c_str());
  Name_Publisher.publish(compName);
  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Subscriber Callback - Name_Subscriber
//# Start Name_Subscriber_OnOneData Marker
void Component_1::Name_Subscriber_OnOneData(const three_component_example::ComponentName::ConstPtr& received_data)
{
  // Business Logic for Name_Subscriber Subscriber
  three_component_example::ComponentName compName;
  compName.name = "Component_1";
  LOGGER.INFO("Component_1::Name_Subscriber::Received Component Name::%s", compName.name.c_str());
}
//# End Name_Subscriber_OnOneData Marker

// Timer Callback - Timer_1
//# Start Timer_1Callback Marker
void Component_1::Timer_1Callback(const ros::TimerEvent& event)
{
  // Business Logic for Timer_1 Timer
  LOGGER.INFO("Component_1::Timer_1::Timer Callback has triggered");
}
//# End Timer_1Callback Marker


// Destructor - Cleanup Ports & Timers
Component_1::~Component_1()
{
  Timer_1.stop();
  Name_Publisher.shutdown();
  Name_Subscriber.shutdown();
  //# Start Destructor Marker
              
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Component_1::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Subscriber - Name_Subscriber
  advertiseName = "ComponentName";
  if (portGroupMap.find("Name_Subscriber") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["Name_Subscriber"];
  ros::SubscribeOptions Name_Subscriber_options;
  Name_Subscriber_options = ros::SubscribeOptions::create<three_component_example::ComponentName>
      (advertiseName.c_str(),
       1000,
       boost::bind(&Component_1::Name_Subscriber_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->Name_Subscriber = nh.subscribe(Name_Subscriber_options);

  // Component Publisher - Name_Publisher
  advertiseName = "ComponentName";
  if (portGroupMap.find("Name_Publisher") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["Name_Publisher"];
  this->Name_Publisher = nh.advertise<three_component_example::ComponentName>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Component_1::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - timer.properties["name"]
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.5),
     boost::bind(&Component_1::Timer_1Callback, this, _1),
     &this->compQueue);
  this->Timer_1 = nh.createTimer(timer_options);

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
    return new Component_1(config,argc,argv);
  }
}
