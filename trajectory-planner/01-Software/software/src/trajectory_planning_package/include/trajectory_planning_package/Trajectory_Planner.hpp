#ifndef TRAJECTORY_PLANNER_HPP
#define TRAJECTORY_PLANNER_HPP
#include "node/Component.hpp"
#include "trajectory_planning_package/sensor_reading.h"
#include "trajectory_planning_package/compute.h"

#ifdef USE_ROSMOD
  #include "rosmod/rosmod_ros.h"
#else
  #ifdef USE_ROSCPP
    #include "ros/ros.h"
  #endif
#endif



//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Trajectory_Planner : public Component
{
public:
  // Constructor
  Trajectory_Planner(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - state_subscriber
  void state_subscriber_operation(const trajectory_planning_package::sensor_reading::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~Trajectory_Planner();

private:

  // Subscriber
  NAMESPACE::Subscriber state_subscriber;

  // Client 
  NAMESPACE::ServiceClient trajectory_client;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
