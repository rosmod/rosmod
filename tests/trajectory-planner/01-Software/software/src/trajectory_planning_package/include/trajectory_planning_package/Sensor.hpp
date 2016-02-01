#ifndef SENSOR_HPP
#define SENSOR_HPP
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

class Sensor : public Component
{
public:
  // Constructor
  Sensor(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Server Operation - trajectory_server
  bool compute_operation(trajectory_planning_package::compute::Request &req, 
    trajectory_planning_package::compute::Response &res);

  // Timer Operation - sensor_timer
  void sensor_timer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Sensor();

private:

  // Timer
  NAMESPACE::Timer sensor_timer;

  // Publisher 
  NAMESPACE::Publisher state_publisher;

  // Server 
  NAMESPACE::ServiceServer trajectory_server;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
