#ifndef ORBITCONTROLLER_HPP
#define ORBITCONTROLLER_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "satellite_flight_application/SatState.h"
#include "cluster_flight_application/TargetOrbit.h"
#include "satellite_flight_application/SatelliteState.h"
#include "satellite_flight_application/ThrusterComm.h"


//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class OrbitController : public Component
{
public:
  // Constructor
  OrbitController(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Subscriber Callback - TargetOrbit_Subscriber
  void TargetOrbit_Subscriber_OnOneData(const cluster_flight_application::TargetOrbit::ConstPtr& received_data); 
 
  // Timer Callback - OrbitController_Timer
  void OrbitController_TimerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~OrbitController();

private:

  // Timer
  ros::Timer OrbitController_Timer;

  // Subscriber
  ros::Subscriber TargetOrbit_Subscriber; 

  // Publisher 
  ros::Publisher SatState_Publisher;

  // Client 
  ros::ServiceClient SatelliteState_Client;

  // Client 
  ros::ServiceClient ThrusterComm_Client;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
