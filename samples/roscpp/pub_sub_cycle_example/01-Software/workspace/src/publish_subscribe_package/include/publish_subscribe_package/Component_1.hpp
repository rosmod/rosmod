#ifndef COMPONENT_1_HPP
#define COMPONENT_1_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "publish_subscribe_package/Message.h"
#include "publish_subscribe_package/Message.h"
//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Component_1 : public Component
{
public:
  // Constructor
  Component_1(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Subscriber Callback - subscriber_port
  void subscriber_port_OnOneData(const publish_subscribe_package::Message::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~Component_1();

private:

  // Subscriber
  ros::Subscriber subscriber_port; 

  // Publisher 
  ros::Publisher publisher_port;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};


#endif
