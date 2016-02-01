#include "ksp_stearwing_controller/Actuator_Component.hpp"

KRPCI krpci_client;

//# Start User Globals Marker
uint64_t vesselID;
uint64_t controlID;
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void Actuator_Component::Init(const ros::TimerEvent& event)
{

  krpci_client.SetIP("191.168.127.100");
  for (int i=0;i<node_argc;i++)
    {
      if (!strcmp(node_argv[i],"--krpc_ip"))
	{
	  krpci_client.SetIP(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--krpc_port"))
	{
	  krpci_client.SetPort(atoi(node_argv[i+1]));
	}      
    }

  // Initialize Here
  if (krpci_client.Connect()) {
    krpci_client.get_ActiveVessel(vesselID);
    krpci_client.Vessel_get_Control(vesselID, controlID);
  }
  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Subscriber Callback - actuator_control_subscriber
//# Start actuator_control_subscriber_OnOneData Marker
void Actuator_Component::actuator_control_subscriber_OnOneData(const ksp_stearwing_controller::Actuation_Command::ConstPtr& received_data)
{
  // Business Logic for actuator_control_subscriber Subscriber
  float new_pitch = received_data->new_pitch;
  float new_roll = received_data->new_roll;
  float new_yaw = received_data->new_yaw;
  float new_throttle = received_data->new_throttle;

  krpci_client.Control_set_Pitch(controlID, new_pitch);
  krpci_client.Control_set_Roll(controlID, new_roll);
  krpci_client.Control_set_Yaw(controlID, new_yaw);
  krpci_client.Control_set_Throttle(controlID, new_throttle);
  LOGGER.INFO("Received Control Commands: Pitch=%f, Roll=%f, Yaw=%f, Throttle=%f", new_pitch, new_roll, new_yaw, new_throttle);
}
//# End actuator_control_subscriber_OnOneData Marker


// Destructor - Cleanup Ports & Timers
Actuator_Component::~Actuator_Component()
{
  actuator_control_subscriber.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Actuator_Component::startUp()
{
  LOGGER.DEBUG("Entering Actuator_Component::startUp");
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Subscriber - actuator_control_subscriber
  advertiseName = "Actuation_Command";
  if (portGroupMap.find("actuator_control_subscriber") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["actuator_control_subscriber"];
  ros::SubscribeOptions actuator_control_subscriber_options;
  actuator_control_subscriber_options = ros::SubscribeOptions::create<ksp_stearwing_controller::Actuation_Command>
      (advertiseName.c_str(),
       1000,
       boost::bind(&Actuator_Component::actuator_control_subscriber_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->actuator_control_subscriber = nh.subscribe(actuator_control_subscriber_options);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Actuator_Component::Init, this, _1),
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

  krpci_client.SetName(nodeName + "_" + compName);
  LOGGER.DEBUG("Exiting Actuator_Component::startUp");
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new Actuator_Component(config,argc,argv);
  }
}
