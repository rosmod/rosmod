#ifndef SECRET_CLIENT_HPP
#define SECRET_CLIENT_HPP
#include "node/Component.hpp"
#include "ten_component/secret_service.h"

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

class Secret_Client : public Component
{
public:
  // Constructor
  Secret_Client(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Timer Operation - secret_timer
  void secret_timer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Secret_Client();

private:

  // Timer
  NAMESPACE::Timer secret_timer;

  // Client 
  NAMESPACE::ServiceClient secret_client;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
