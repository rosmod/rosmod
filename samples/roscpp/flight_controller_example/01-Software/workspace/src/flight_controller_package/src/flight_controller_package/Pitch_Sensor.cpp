#include "flight_controller_package/Pitch_Sensor.hpp"

//# Start User Globals Marker
uint64_t vesselID;
uint64_t refFrame;
uint64_t flightID;
float pitch;
//# End User Globals Marker

KRPCI krpci_client;

// Initialization Function
//# Start Init Marker
void Pitch_Sensor::Init(const ros::TimerEvent& event)
{
  std::cout << "About to Connect" << std::endl;
  krpci_client.SetName(nodeName + "_" + compName);

  // Initialize Here
  if (krpci_client.Connect()) {
    // Get Active Vessel
    krpci_client.get_ActiveVessel(vesselID);

    std::cout << "Vessel ID: " << vesselID << std::endl;

    // Get Reference Frame
    krpci_client.Vessel_get_SurfaceReferenceFrame(vesselID, refFrame);

    std::cout << "Surface Reference Frame: " << refFrame << std::endl;

    // Get Flight ID
    krpci_client.Vessel_Flight(vesselID, refFrame, flightID);

    std::cout << "FlightID: " << flightID << std::endl;
  }

  // Stop Init Timer
  initOneShotTimer.stop();
}
//# End Init Marker

// Timer Callback - pitch_sensor_timer
//# Start pitch_sensor_timerCallback Marker
void Pitch_Sensor::pitch_sensor_timerCallback(const ros::TimerEvent& event)
{
  // Business Logic for pitch_sensor_timer Timer
  krpci_client.Flight_get_Pitch(flightID, pitch);

  flight_controller_package::Pitch new_pitch;
  new_pitch.value = pitch;
  pitch_publisher.publish(new_pitch);
  LOGGER.INFO("Pitch_Publisher::New Pitch Value: %f", new_pitch.value);
}
//# End pitch_sensor_timerCallback Marker


// Destructor - Cleanup Ports & Timers
Pitch_Sensor::~Pitch_Sensor()
{
  pitch_sensor_timer.stop();
  pitch_publisher.shutdown();
  //# Start Destructor Marker
                        krpci_client.Close();
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void Pitch_Sensor::startUp()
{
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Publisher - pitch_publisher
  advertiseName = "Pitch";
  if (portGroupMap.find("pitch_publisher") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["pitch_publisher"];
  this->pitch_publisher = nh.advertise<flight_controller_package::Pitch>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&Pitch_Sensor::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - pitch_sensor_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.2),
     boost::bind(&Pitch_Sensor::pitch_sensor_timerCallback, this, _1),
     &this->compQueue);
  this->pitch_sensor_timer = nh.createTimer(timer_options);

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
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new Pitch_Sensor(config,argc,argv);
  }
}
