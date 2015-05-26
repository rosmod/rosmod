#ifndef COMPONENT_A_HPP
#define COMPONENT_A_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "deadlock/common_service.h"
#include "deadlock/common_service.h"
//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Component_A : public Component
{
public:
  // Constructor
  Component_A(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Server Callback - server_A
  bool common_serviceCallback(deadlock::common_service::Request &req, 
    deadlock::common_service::Response &res);

  // Timer Callback - timer_A
  void timer_ACallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Component_A();

private:

  // Timer
  ros::Timer timer_A;

  // Server 
  ros::ServiceServer server_A;

  // Client 
  ros::ServiceClient client_A;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};


#endif
