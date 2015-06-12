#include "sequential_leds_app/led_controller.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void led_controller::Init(const ros::TimerEvent& event)
{
  // Initialize Here
  ledPin = 0;  // if using USR LEDs, use 0-3, else use real pin values
  ledState = false;
  
  for (int i = 0; i < node_argc; i++)
    {
      if (!strcmp(node_argv[i], "--pin"))
	{
	  ledPin = atoi(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i], "--start"))
	{
	  // publish here
	}
    }
  (ledState) ? led_set_value(ledPin, HIGH) : led_set_value(ledPin, LOW);
  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Subscriber Callback - led_state_sub
//# Start led_state_sub_OnOneData Marker
void led_controller::led_state_sub_OnOneData(const sequential_leds_app::led_state::ConstPtr& received_data)
{
  // Business Logic for led_state_sub Subscriber
}
//# End led_state_sub_OnOneData Marker


// Destructor - Cleanup Ports & Timers
led_controller::~led_controller()
{
  led_state_pub.shutdown();
  led_state_sub.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void led_controller::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Subscriber - led_state_sub
  advertiseName = "led_state";
  if (portGroupMap.find("led_state_sub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["led_state_sub"];
  ros::SubscribeOptions led_state_sub_options;
  led_state_sub_options = ros::SubscribeOptions::create<sequential_leds_app::led_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&led_controller::led_state_sub_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->led_state_sub = nh.subscribe(led_state_sub_options);

  // Component Publisher - led_state_pub
  advertiseName = "led_state";
  if (portGroupMap.find("led_state_pub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["led_state_pub"];
  this->led_state_pub = nh.advertise<sequential_leds_app::led_state>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&led_controller::Init, this, _1),
     &this->compQueue,
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
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new led_controller(config,argc,argv);
  }
}
