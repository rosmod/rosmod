#ifndef TRAJECTORY_PLANNER_HPP
#define TRAJECTORY_PLANNER_HPP
#include "rosmod/rosmod_ros.h"
#include "Component.hpp"
#include "trajectory_planning_package/sensor_reading.h"
#include "trajectory_planning_package/compute.h"


//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Trajectory_Planner : public Component
{
public:
  // Constructor
  Trajectory_Planner(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const rosmod::TimerEvent& event);

  // Subscriber Callback - state_subscriber
  void state_subscriber_OnOneData(const trajectory_planning_package::sensor_reading::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~Trajectory_Planner();

private:

  // Subscriber
  rosmod::Subscriber state_subscriber; 

  // Client 
  rosmod::ServiceClient trajectory_client;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
