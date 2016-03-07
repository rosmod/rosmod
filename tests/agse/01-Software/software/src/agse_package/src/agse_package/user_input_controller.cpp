#include "agse_package/user_input_controller.hpp"

//# Start User Globals Marker
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void user_input_controller::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering user_input_controller::init_timer_operation");
#endif
  // Initialize Here
  paused = false;
  halted = false;
  manual = false;
  
  noGPIO = false;

  // Command line args for radial goal
  for (int i = 0; i < node_argc; i++)
    {
      if (!strcmp(node_argv[i], "-noGPIO"))
	{
	  noGPIO = true;
	}
    }

  if (noGPIO == false) {

    //////////////////////////////////////////////
    // UIP SWITCHES
    //////////////////////////////////////////////

    // PAUSE Switch - Amber
    pauseSwitchPin = 27; // P8_17
    gpio_export(pauseSwitchPin);
    gpio_set_dir(pauseSwitchPin,INPUT_PIN);

    // MANUAL Switch - Blue
    manualSwitchPin = 22; // P8_19
    gpio_export(manualSwitchPin);
    gpio_set_dir(manualSwitchPin,INPUT_PIN);

    // HALT Switch - Red
    haltSwitchPin = 65; // P8_18
    gpio_export(haltSwitchPin);
    gpio_set_dir(haltSwitchPin,INPUT_PIN);

    //////////////////////////////////////////////
    // UIP LEDS
    //////////////////////////////////////////////

    // PAUSE MAIN LED 
    pauseLED = 37; // P8_22 - Amber
    gpio_export(pauseLED);
    gpio_set_dir(pauseLED, OUTPUT_PIN);
    pauseLEDBlinkDelay = 5;

    // ALARM MAIN LED
    alarmLED = 66; // P8_07 - Red
    gpio_export(alarmLED);
    gpio_set_dir(alarmLED, OUTPUT_PIN);  

    // INIT MAIN LED
    initLED[0] = 45; // P8_11 - Blue
    initLED[1] = 44; // P8_12 - Green
    initLED[2] = 23; // P8_13 - Red
    gpio_export(initLED[0]);
    gpio_set_dir(initLED[0], OUTPUT_PIN);  
    gpio_export(initLED[1]);
    gpio_set_dir(initLED[1], OUTPUT_PIN);  
    gpio_export(initLED[2]);
    gpio_set_dir(initLED[2], OUTPUT_PIN);  

    gpio_set_value(initLED[0], HIGH);

    // SAMPLE MAIN LED
    sampleLED[0] = 26; // P8_14 - Blue
    sampleLED[1] = 47; // P8_15 - Green
    sampleLED[2] = 46; // P8_16 - Red
    gpio_export(sampleLED[0]);
    gpio_set_dir(sampleLED[0], OUTPUT_PIN);  
    gpio_export(sampleLED[1]);
    gpio_set_dir(sampleLED[1], OUTPUT_PIN);  
    gpio_export(sampleLED[2]);
    gpio_set_dir(sampleLED[2], OUTPUT_PIN);  
    sampleLEDBlinkDelay = 5;

    // BAY MAIN LED
    bayLED[0] = 67; // P8_08 - Blue
    bayLED[1] = 69; // P8_09 - Green
    bayLED[2] = 68; // P8_10 - Red
    gpio_export(bayLED[0]);
    gpio_set_dir(bayLED[0], OUTPUT_PIN);  
    gpio_export(bayLED[1]);
    gpio_set_dir(bayLED[1], OUTPUT_PIN);  
    gpio_export(bayLED[2]);
    gpio_set_dir(bayLED[2], OUTPUT_PIN);  
    bayLEDBlinkDelay = 5;
  }

  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting user_input_controller::init_timer_operation");
#endif  
}
//# End Init Marker



// Subscriber Operation - armState_sub
//# Start armState_sub_operation Marker
void user_input_controller::armState_sub_operation(const agse_package::armState::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering user_input_controller::armState_sub_operation");
#endif
  // Business Logic for armState_sub_operation
  arm.state = received_data->state;
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting user_input_controller::armState_sub_operation");
#endif
}
//# End armState_sub_operation Marker
// Subscriber Operation - sampleState_sub
//# Start sampleState_sub_operation Marker
void user_input_controller::sampleState_sub_operation(const agse_package::sampleState::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering user_input_controller::sampleState_sub_operation");
#endif
  // Business Logic for sampleState_sub_operation
  sample.pos = received_data->pos;
  sample.orientation = received_data->orientation;
  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting user_input_controller::sampleState_sub_operation");
#endif
}
//# End sampleState_sub_operation Marker
// Subscriber Operation - payloadBayState_sub
//# Start payloadBayState_sub_operation Marker
void user_input_controller::payloadBayState_sub_operation(const agse_package::payloadBayState::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering user_input_controller::payloadBayState_sub_operation");
#endif
  // Business Logic for payloadBayState_sub_operation
  payloadBay.pos = received_data->pos;
  payloadBay.orientation = received_data->orientation;
  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting user_input_controller::payloadBayState_sub_operation");
#endif
}
//# End payloadBayState_sub_operation Marker

// Timer Callback - userInputTimer
//# Start userInputTimer_operation Marker
void user_input_controller::userInputTimer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering user_input_controller::userInputTimer_operation");
#endif
  // Business Logic for userInputTimer_operation
  if (noGPIO == false) {

    // Business Logic for userInputTimer
    // HANDLE MISSILE SWITCHES HERE
    unsigned int previousSwitchState = pauseSwitchState;
    gpio_get_value(pauseSwitchPin, &pauseSwitchState);
    //  ROS_INFO("Pause Switch State: %d", pauseSwitchState);

    agse_package::controlInputs control;
  
    if ( previousSwitchState != pauseSwitchState )
      {
	paused = (pauseSwitchState == HIGH) ? true : false;
	if (paused) {
	  ROS_INFO("Pausing the system!");
	}
	else {
	  ROS_INFO("Unpausing the system!");
	}
      }
    previousSwitchState = haltSwitchState;
    gpio_get_value(haltSwitchPin, &haltSwitchState);
    if ( previousSwitchState != haltSwitchState )
      {
	halted = (haltSwitchState == HIGH) ? true : false;
	if (halted) {
	  ROS_INFO("Halting the system!");
	}
	else {
	  ROS_INFO("Un-halting the system!");
	}
      }
    previousSwitchState = manualSwitchState;
    gpio_get_value(manualSwitchPin, &manualSwitchState);
    if ( previousSwitchState != manualSwitchState )
      {
	manual= (manualSwitchState == HIGH) ? true : false;
	if (manual) {
	  ROS_INFO("Switching the system to manual!");
	}
	else {
	  ROS_INFO("Switching the system to automatic!");
	}
      }
    control.paused = paused;
    control.stop = halted;
    control.manual = manual;
    controlInputs_pub.publish(control);

    // HANDLE LED OUTPUTS HERE
    static int currentPauseLEDBlinkDelay = 0;
    static int currentSampleLEDBlinkDelay = 0;
    static int currentBayLEDBlinkDelay = 0;

    if (paused)
      {
	gpio_set_value(pauseLED, HIGH);
      }
    else
      {
	if (currentPauseLEDBlinkDelay++ < pauseLEDBlinkDelay)
	  gpio_set_value(pauseLED,LOW);
	else
	  {
	    gpio_set_value(pauseLED,HIGH);
	    currentPauseLEDBlinkDelay = 0;
	  }
      }

    if (halted) {
      gpio_set_value(alarmLED, HIGH);
    }
    else {
      gpio_set_value(alarmLED, LOW);
    }

    switch (arm.state) {
    case 0:
      // INIT
      gpio_set_value(initLED[0], HIGH);
      break;
    case 1:
      // OPENING_PB
      gpio_set_value(initLED[0], LOW); // Switch OFF Blue
      gpio_set_value(initLED[1], HIGH); // Switch ON Green
      gpio_set_value(bayLED[0], LOW); // Switch OFF Blue
      //    gpio_set_value(bayLED[1], HIGH); // Switch ON Green    
      break;
    case 2:
      // FINDING_SAMPLE
      gpio_set_value(sampleLED[0], HIGH); // Switch ON Blue
      break;
    case 3:
      // FINDING_PB
      gpio_set_value(bayLED[0], HIGH); // Blue

      // Blink Sample LED Green
      gpio_set_value(sampleLED[0], LOW); // Switch ON Blue
      if (currentSampleLEDBlinkDelay++ < sampleLEDBlinkDelay)
	gpio_set_value(sampleLED[1], LOW);
      else
	{
	  gpio_set_value(sampleLED[1], HIGH);
	  currentSampleLEDBlinkDelay = 0;
	}
      break;
    case 4:
      // GRABBING_SAMPLE
      gpio_set_value(sampleLED[0], LOW); // Switch OFF Blue
      gpio_set_value(sampleLED[1], HIGH); // Switch ON Green    

      // Blink Payload Bay LED Gree
      gpio_set_value(bayLED[0], LOW); // Switch OFF Blue
      if (currentBayLEDBlinkDelay++ < bayLEDBlinkDelay)
	gpio_set_value(bayLED[1], LOW);
      else
	{
	  gpio_set_value(bayLED[1], HIGH);
	  currentBayLEDBlinkDelay = 0;
	}
      break;
    case 5:
      gpio_set_value(bayLED[1], HIGH);
      gpio_set_value(sampleLED[1], HIGH);
      break;
    default:
      break;
    }
  }
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting user_input_controller::userInputTimer_operation");
#endif
}
//# End userInputTimer_operation Marker


// Destructor - Cleanup Ports & Timers
user_input_controller::~user_input_controller()
{
  userInputTimer.stop();
  controlInputs_pub.shutdown();
  armState_sub.shutdown();
  sampleState_sub.shutdown();
  payloadBayState_sub.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void user_input_controller::startUp()
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

  logger->create_file("/var/log/" + config.nodeName + "." + config.compName + ".log");
  logger->set_is_periodic(config.is_periodic_logging);
  logger->set_max_log_unit(config.periodic_log_unit);

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->create_file("/var/log/ROSMOD_DEBUG." + config.nodeName + "." + config.compName + ".log");
  comp_queue.ROSMOD_LOGGER->set_is_periodic(config.is_periodic_logging);
  comp_queue.ROSMOD_LOGGER->set_max_log_unit(config.periodic_log_unit);
#endif    
  
#ifdef USE_ROSMOD 
  this->comp_queue.scheduling_scheme = config.schedulingScheme;
  rosmod::ROSMOD_Callback_Options callback_options;
#endif  
  // Configure all publishers associated with this component
  // Component Publisher - controlInputs_pub
  advertiseName = "controlInputs";
  if (config.portGroupMap.find("controlInputs_pub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["controlInputs_pub"];
  this->controlInputs_pub = nh.advertise<agse_package::controlInputs>(advertiseName.c_str(), 1000);


  if (config.num_comps_to_sync > 1 )
    {
      // Synchronize components now that all publishers and servers have been initialized
      this->comp_sync_pub = nh.advertise<std_msgs::Bool>("component_synchronization", 1000);
  
#ifdef USE_ROSMOD  
      rosmod::SubscribeOptions comp_sync_sub_options;
      rosmod::ROSMOD_Callback_Options sync_callback_options;
#else
      ros::SubscribeOptions comp_sync_sub_options;
#endif
      ros::Duration(config.comp_sync_timeout/2.0).sleep();
      comp_sync_sub_options = NAMESPACE::SubscribeOptions::create<std_msgs::Bool>
	("component_synchronization",
	 1000,
	 boost::bind(&user_input_controller::component_sync_operation, this, _1),
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
      ros::Duration(config.comp_sync_timeout/2.0).sleep();
      this->comp_sync_sub.shutdown();  
      this->comp_sync_pub.shutdown();
    }

  // Configure all subscribers associated with this component
#ifdef USE_ROSMOD 
  callback_options.alias = "armState_sub_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif  
  // Component Subscriber - armState_sub
  advertiseName = "armState";
  if (config.portGroupMap.find("armState_sub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["armState_sub"];
  NAMESPACE::SubscribeOptions armState_sub_options;
  armState_sub_options = NAMESPACE::SubscribeOptions::create<agse_package::armState>
      (advertiseName.c_str(),
       1000,
       boost::bind(&user_input_controller::armState_sub_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  armState_sub_options.transport_hints = NAMESPACE::TransportHints().udp();
  this->armState_sub = nh.subscribe(armState_sub_options);
#ifdef USE_ROSMOD 
  callback_options.alias = "sampleState_sub_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif  
  // Component Subscriber - sampleState_sub
  advertiseName = "sampleState";
  if (config.portGroupMap.find("sampleState_sub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["sampleState_sub"];
  NAMESPACE::SubscribeOptions sampleState_sub_options;
  sampleState_sub_options = NAMESPACE::SubscribeOptions::create<agse_package::sampleState>
      (advertiseName.c_str(),
       1000,
       boost::bind(&user_input_controller::sampleState_sub_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  sampleState_sub_options.transport_hints = NAMESPACE::TransportHints().udp();
  this->sampleState_sub = nh.subscribe(sampleState_sub_options);
#ifdef USE_ROSMOD 
  callback_options.alias = "payloadBayState_sub_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif  
  // Component Subscriber - payloadBayState_sub
  advertiseName = "payloadBayState";
  if (config.portGroupMap.find("payloadBayState_sub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["payloadBayState_sub"];
  NAMESPACE::SubscribeOptions payloadBayState_sub_options;
  payloadBayState_sub_options = NAMESPACE::SubscribeOptions::create<agse_package::payloadBayState>
      (advertiseName.c_str(),
       1000,
       boost::bind(&user_input_controller::payloadBayState_sub_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  payloadBayState_sub_options.transport_hints = NAMESPACE::TransportHints().udp();
  this->payloadBayState_sub = nh.subscribe(payloadBayState_sub_options);

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
     boost::bind(&user_input_controller::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();
#ifdef USE_ROSMOD   
  callback_options.alias = "userInputTimer_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif
  // Component Timer - userInputTimer
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(0.1),
     boost::bind(&user_input_controller::userInputTimer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif 
     false,
     false);
  this->userInputTimer = nh.createTimer(timer_options);


  this->init_timer.start();
  this->userInputTimer.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new user_input_controller(config,argc,argv);
  }
}
