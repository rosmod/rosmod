#include "client_server/led_service_comp.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void led_service_comp::Init(const ros::TimerEvent& event)
{
  // Initialize Here
  ledPin = 0;  // if using USR LEDs, use 0-3, else use real pin values
  useGPIO = true;
  ledState = false;
  invert = false;
 
  bool start = false;
  
  for (int i = 0; i < node_argc; i++)
    {
      if (!strcmp(node_argv[i], "--pin"))
	{
	  ledPin = atoi(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i], "--invert"))
	{
	  invert = true;
	}
      if (!strcmp(node_argv[i], "--start"))
	{
	  start = true;
	}
      if (!strcmp(node_argv[i], "--nogpio"))
	{
	  useGPIO = false;
	}
    }

  //if (useGPIO)
    //(ledState) ? led_set_value(ledPin, HIGH) : led_set_value(ledPin, LOW);

  if (start)
    {
      ledState = true;
      //if (useGPIO)
	//(ledState) ? led_set_value(ledPin, HIGH) : led_set_value(ledPin, LOW);
    }
  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Server Callback - led_server
//# Start led_stateCallback Marker
bool led_service_comp::led_stateCallback(client_server::led_state::Request  &req,
  client_server::led_state::Response &res)
{
  // Business Logic for led_server Server
  LOGGER.INFO("Server got request from client.\n");
  ledState = req.state;
  ledPin = req.pin;
  if (invert)
    ledState = !ledState;
  return true;
}
//# End led_stateCallback Marker

// Timer Callback - ledTimer
//# Start ledTimerCallback Marker
void led_service_comp::ledTimerCallback(const ros::TimerEvent& event)
{
  //if (useGPIO)
    //(ledState) ? led_set_value(ledPin, HIGH) : led_set_value(ledPin, LOW);
  // Business Logic for ledTimer Timer
  client_server::led_state ledStateFunction;
  ledStateFunction.request.pin = ledPin;
  ledStateFunction.request.state = ledState;
  if (led_client.call(ledStateFunction))
    LOGGER.INFO("Client call succeeded.\n");
  else
    LOGGER.INFO("Client call failed.\n");
}
//# End ledTimerCallback Marker


// Destructor - Cleanup Ports & Timers
led_service_comp::~led_service_comp()
{
  ledTimer.stop();
  led_server.shutdown();
  led_client.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void led_service_comp::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Server - led_server
  advertiseName = "led_state";
  if (portGroupMap.find("led_server") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["led_server"];
  ros::AdvertiseServiceOptions led_server_server_options;
  led_server_server_options = ros::AdvertiseServiceOptions::create<client_server::led_state>
      (advertiseName.c_str(),
       boost::bind(&led_service_comp::led_stateCallback, this, _1, _2),
       ros::VoidPtr(),
       &this->compQueue);
  this->led_server = nh.advertiseService(led_server_server_options);
 
  // Configure all required services associated with this component
  // Component Client - led_client
  advertiseName = "led_state";
  if (portGroupMap.find("led_client") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["led_client"];
      this->led_client = nh.serviceClient<client_server::led_state>(advertiseName.c_str()); 

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&led_service_comp::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - ledTimer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.5),
     boost::bind(&led_service_comp::ledTimerCallback, this, _1),
     &this->compQueue);
  this->ledTimer = nh.createTimer(timer_options);

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
    return new led_service_comp(config,argc,argv);
  }
}
