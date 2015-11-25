#ifndef SERVER_HPP
#define SERVER_HPP
#include "node/Component.hpp"
#include "client_server_package/Power.h"

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

class Server : public Component
{
public:
  // Constructor
  Server(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Server Operation - server_port
  bool Power_operation(client_server_package::Power::Request &req, 
    client_server_package::Power::Response &res);

  // Start up
  void startUp();

  // Destructor
  ~Server();

private:

  // Server 
  NAMESPACE::ServiceServer server_port;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
