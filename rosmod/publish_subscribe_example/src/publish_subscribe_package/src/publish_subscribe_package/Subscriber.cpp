#include "publish_subscribe_package/Subscriber.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Subscriber::Init(const rosmod::TimerEvent& event)
{
  compQueue.ROSMOD_LOGGER.DEBUG("Entering Timer::Init");  
  // Initialize Here

  // Stop Init Timer
  initOneShotTimer.stop();
  compQueue.ROSMOD_LOGGER.DEBUG("Exiting Timer::Init");  
}
//# End Init Marker

// Subscriber Callback - subscriber_port
//# Start subscriber_port_OnOneData Marker
void Subscriber::subscriber_port_OnOneData(const publish_subscribe_package::Message::ConstPtr& received_data)
{
  compQueue.ROSMOD_LOGGER.DEBUG("Entering subscriber_port_OnOneData");  
  // Business Logic for subscriber_port Subscriber
  LOGGER.INFO("Subscriber::Subscriber_Port::Received Message - %s", received_data->name.c_str());
  compQueue.ROSMOD_LOGGER.DEBUG("Exiting subscriber_port_OnOneData");
}
//# End subscriber_port_OnOneData Marker


// Destructor - Cleanup Ports & Timers
Subscriber::~Subscriber()
{
  subscriber_port.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Subscriber::startUp()
{
  rosmod::NodeHandle nh;
  std::string advertiseName;

  // Scheduling Scheme is FIFO
  this->compQueue.scheduling_scheme = rosmod::CallbackQueue::SchedulingScheme::FIFO;
    
  rosmod::ROSMOD_Callback_Options callback_options;
  callback_options.alias = "SubscriberCallback";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0.0;
  callback_options.deadline.nsec = 40000; 

  // Component Subscriber - subscriber_port
  advertiseName = "Message";
  if (portGroupMap.find("subscriber_port") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["subscriber_port"];
  rosmod::SubscribeOptions subscriber_port_options;
  subscriber_port_options = rosmod::SubscribeOptions::create<publish_subscribe_package::Message>
      (advertiseName.c_str(),
       1000,
       boost::bind(&Subscriber::subscriber_port_OnOneData, this, _1),
       rosmod::VoidPtr(),
       &this->compQueue,
       callback_options);
  this->subscriber_port = nh.subscribe(subscriber_port_options);

  callback_options.alias = "Init Timer";
  
  // Init Timer
  rosmod::TimerOptions timer_options;
  timer_options = 
    rosmod::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Subscriber::Init, this, _1),
     &this->compQueue,
     callback_options,
     true,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
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

  compQueue.ROSMOD_LOGGER.CREATE_FILE(pwd + "ROSMOD_DEBUG." + nodeName + "." + compName + ".log"); 
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new Subscriber(config,argc,argv);
  }
}
