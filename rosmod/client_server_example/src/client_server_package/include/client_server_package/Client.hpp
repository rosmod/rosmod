#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "rosmod/rosmod_ros.h"
#include "Component.hpp"
#include "client_server_package/Power.h"
//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Client : public Component
{
public:
  // Constructor
  Client(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const rosmod::TimerEvent& event);

  // Timer Callback - client_timer
  void client_timerCallback(const rosmod::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Client();

private:

  // Timer
  rosmod::Timer client_timer;

  // Client 
  rosmod::ServiceClient client_port;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};


#endif
