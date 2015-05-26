#ifndef PUBLISHER_HPP
#define PUBLISHER_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "publish_subscribe_package/Message.h"
//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Publisher : public Component
{
public:
  // Constructor
  Publisher(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Timer Callback - publish_timer
  void publish_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Publisher();

private:

  // Timer
  ros::Timer publish_timer;

  // Publisher 
  ros::Publisher publisher_port;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};


#endif
