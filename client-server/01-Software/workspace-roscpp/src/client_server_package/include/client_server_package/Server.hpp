#ifndef SERVER_HPP
#define SERVER_HPP
#include "ros/ros.h"
#include "Component.hpp"
#include "client_server_package/Power.h"


//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Server : public Component
{
public:
  // Constructor
  Server(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Server Callback - server_port
  bool PowerCallback(client_server_package::Power::Request &req, 
    client_server_package::Power::Response &res);

  // Start up
  void startUp();

  // Destructor
  ~Server();

private:

  // Server 
  ros::ServiceServer server_port;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
