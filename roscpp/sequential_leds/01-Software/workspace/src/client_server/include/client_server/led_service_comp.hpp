#ifndef LED_SERVICE_COMP_HPP
#define LED_SERVICE_COMP_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "client_server/led_state.h"
#include "client_server/led_state.h"
//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class led_service_comp : public Component
{
public:
  // Constructor
  led_service_comp(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Server Callback - led_server
  bool led_stateCallback(client_server::led_state::Request &req, 
    client_server::led_state::Response &res);

  // Timer Callback - ledTimer
  void ledTimerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~led_service_comp();

private:

  // Timer
  ros::Timer ledTimer;

  // Server 
  ros::ServiceServer led_server;

  // Client 
  ros::ServiceClient led_client;

  //# Start User Private Variables Marker
  bool ledState;
  bool invert;
  unsigned int ledPin;
  bool useGPIO;
  //# End User Private Variables Marker
};


#endif
