#ifndef TRAJECTORYPLANNER_HPP
#define TRAJECTORYPLANNER_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "cluster_flight_application/TargetOrbit.h"
#include "satellite_flight_application/SatState.h"
#include "satellite_flight_application/GroundCommand.h"


//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class TrajectoryPlanner : public Component
{
public:
  // Constructor
  TrajectoryPlanner(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Subscriber Callback - SatState_Subscriber
  void SatState_Subscriber_OnOneData(const satellite_flight_application::SatState::ConstPtr& received_data); 
 
  // Subscriber Callback - GroundCommand_Subscriber
  void GroundCommand_Subscriber_OnOneData(const satellite_flight_application::GroundCommand::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~TrajectoryPlanner();

private:

  // Subscriber
  ros::Subscriber SatState_Subscriber; 

  // Subscriber
  ros::Subscriber GroundCommand_Subscriber; 

  // Publisher 
  ros::Publisher TargetOrbit_Publisher;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
