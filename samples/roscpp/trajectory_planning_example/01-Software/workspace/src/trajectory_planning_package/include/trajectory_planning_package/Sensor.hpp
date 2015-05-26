#ifndef SENSOR_HPP
#define SENSOR_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "trajectory_planning_package/sensor_reading.h"
#include "trajectory_planning_package/compute.h"
//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Sensor : public Component
{
public:
  // Constructor
  Sensor(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Server Callback - trajectory_server
  bool computeCallback(trajectory_planning_package::compute::Request &req, 
    trajectory_planning_package::compute::Response &res);

  // Timer Callback - sensor_timer
  void sensor_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Sensor();

private:

  // Timer
  ros::Timer sensor_timer;

  // Publisher 
  ros::Publisher state_publisher;

  // Server 
  ros::ServiceServer trajectory_server;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};


#endif
