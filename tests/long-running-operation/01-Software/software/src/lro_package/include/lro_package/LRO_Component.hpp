#ifndef LRO_COMPONENT_HPP
#define LRO_COMPONENT_HPP
#include "node/Component.hpp"
#include "lro_package/Message.h"

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

class LRO_Component : public Component
{
public:
  // Constructor
  LRO_Component(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - critical_subscriber_port
  void critical_subscriber_port_operation(const lro_package::Message::ConstPtr& received_data); 
 
  // Timer Operation - long_running_operation
  void long_running_operation_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~LRO_Component();

private:

  // Timer
  NAMESPACE::Timer long_running_operation;

  // Subscriber
  NAMESPACE::Subscriber critical_subscriber_port;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
