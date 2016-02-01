#ifndef SUBSCRIBER_HPP
#define SUBSCRIBER_HPP
#include "node/Component.hpp"
#include "publish_subscribe_package/Message.h"

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

class Subscriber : public Component
{
public:
  // Constructor
  Subscriber(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - subscriber_port
  void subscriber_port_operation(const publish_subscribe_package::Message::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~Subscriber();

private:

  // Subscriber
  NAMESPACE::Subscriber subscriber_port;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
