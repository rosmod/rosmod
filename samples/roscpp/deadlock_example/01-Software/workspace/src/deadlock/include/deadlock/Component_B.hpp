#ifndef COMPONENT_B_HPP
#define COMPONENT_B_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "deadlock/common_service.h"
#include "deadlock/common_service.h"
//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Component_B : public Component
{
public:
  // Constructor
  Component_B(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Server Callback - server_B
  bool common_serviceCallback(deadlock::common_service::Request &req, 
    deadlock::common_service::Response &res);

  // Start up
  void startUp();

  // Destructor
  ~Component_B();

private:

  // Server 
  ros::ServiceServer server_B;

  // Client 
  ros::ServiceClient client_B;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};


#endif
