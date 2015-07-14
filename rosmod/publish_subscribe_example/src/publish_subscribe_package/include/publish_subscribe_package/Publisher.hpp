#ifndef PUBLISHER_HPP
#define PUBLISHER_HPP

#include "rosmod/rosmod_ros.h"
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
  void Init(const rosmod::TimerEvent& event);

  // Timer Callback - publish_timer
  void publish_timerCallback(const rosmod::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Publisher();

private:

  // Timer
  rosmod::Timer publish_timer;

  // Publisher 
  rosmod::Publisher publisher_port;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};


#endif
