#ifndef COMPONENT_1_HPP
#define COMPONENT_1_HPP
#include "rosmod/rosmod_ros.h"
#include "node/Component.hpp"
#include "three_component_example/ComponentName.h"
#include "three_component_example/ComponentName.h"



//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Component_1 : public Component
{
public:
  // Constructor
  Component_1(ComponentConfig& _config, int argc, char **argv) : Component(_config, argc, argv) {}

  // Initialization
  void Init(const rosmod::TimerEvent& event);

  // Subscriber Callback - Name_Subscriber
  void Name_Subscriber_OnOneData(const three_component_example::ComponentName::ConstPtr& received_data); 
 
  // Timer Callback - Timer_1
  void Timer_1Callback(const rosmod::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Component_1();

private:

  // Timer
  rosmod::Timer Timer_1;

  // Subscriber
  rosmod::Subscriber Name_Subscriber;

  // Publisher 
  rosmod::Publisher Name_Publisher;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
