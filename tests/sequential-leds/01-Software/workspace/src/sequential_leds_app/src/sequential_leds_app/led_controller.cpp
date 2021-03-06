#include "sequential_leds_app/led_controller.hpp"

//# Start User Globals Marker
void led_mode(std::string mode, int leds, float delay)
{
  static PIN_VALUE ledState = LOW;
  if ( mode == "toggle" )
    {
      ledState = (ledState == HIGH) ? LOW : HIGH;
    }
  for (int i = 0; i < leds; i++)
    led_set_value(i, ledState);
  if ( mode == "blink" )
    {
      ros::Duration(0.5).sleep();
      ledState = (ledState == HIGH) ? LOW : HIGH;
      for (int i = 0; i < leds; i++)
	led_set_value(i, ledState);
      ledState = (ledState == HIGH) ? LOW : HIGH; // set up for next execution
    }
}
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void led_controller::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering led_controller::init_timer_operation");
#endif
  // Initialize Here
  led_num = 0;
  useGPIO = true;
  invert = false;
  publish = false;

  unsigned int numLEDs = 4;  // if using USR LEDs, use 0-3, else use real pin values
  std::string mode = "toggle";
  float delayTime;
 
  bool start = false;
  
  for (int i = 0; i < node_argc; i++)
    {
      if (!strcmp(node_argv[i], "--numleds"))
	{
	  numLEDs = atoi(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i], "--led_num"))
	{
	  led_num = atoi(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i], "--end"))
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
      if (!strcmp(node_argv[i], "--delay"))
	{
	  delayTime = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i], "--publish"))
	{
	  publish = true;
	}
      if (!strcmp(node_argv[i], "--mode"))
	{
	  mode = node_argv[i+1];
	}
    }

  if (start)
    {
      if (useGPIO)
	led_mode(mode, numLEDs, delayTime);

      sequential_leds_app::led_state nextLEDState;
      nextLEDState.numLEDs = numLEDs;
      nextLEDState.led_num = led_num + 1;
      nextLEDState.delay = delayTime;
      nextLEDState.mode = mode;
      led_state_pub.publish(nextLEDState);
    }
  if (useGPIO)
    for (int i = 0; i < numLEDs; i++)
      led_set_value(i, LOW);
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting led_controller::init_timer_operation");
#endif  
}
//# End Init Marker



// Subscriber Operation - led_state_sub
//# Start led_state_sub_operation Marker
void led_controller::led_state_sub_operation(const sequential_leds_app::led_state::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering led_controller::led_state_sub_operation");
#endif
  // Business Logic for led_state_sub_operation
  if ( received_data->led_num == led_num )
    {
      if (useGPIO)
	led_mode(received_data->mode, 
		 received_data->numLEDs, 
		 received_data->delay);
      
      if (publish)
	{
	  sequential_leds_app::led_state nextLEDState;
	  nextLEDState.numLEDs = received_data->numLEDs;
	  nextLEDState.delay = received_data->delay;
	  nextLEDState.mode = received_data->mode;
	  if (!invert)
	    nextLEDState.led_num = led_num + 1;
	  else
	    nextLEDState.led_num = 0;
	  led_state_pub.publish(nextLEDState);
	}
    }
  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting led_controller::led_state_sub_operation");
#endif
}
//# End led_state_sub_operation Marker


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
  // Component Publisher - led_state_pub
  advertiseName = "led_state";
  if (config.portGroupMap.find("led_state_pub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["led_state_pub"];
  this->led_state_pub = nh.advertise<sequential_leds_app::led_state>(advertiseName.c_str(), 1000);


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
     boost::bind(&led_controller::component_sync_operation, this, _1),
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
  callback_options.alias = "led_state_sub_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif  
  // Component Subscriber - led_state_sub
  advertiseName = "led_state";
  if (config.portGroupMap.find("led_state_sub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["led_state_sub"];
  NAMESPACE::SubscribeOptions led_state_sub_options;
  led_state_sub_options = NAMESPACE::SubscribeOptions::create<sequential_leds_app::led_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&led_controller::led_state_sub_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  this->led_state_sub = nh.subscribe(led_state_sub_options);

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
     boost::bind(&led_controller::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();


  this->init_timer.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new led_controller(config,argc,argv);
  }
}
