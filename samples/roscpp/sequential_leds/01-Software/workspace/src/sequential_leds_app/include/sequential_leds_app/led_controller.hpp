#ifndef LED_CONTROLLER_HPP
#define LED_CONTROLLER_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "sequential_leds_app/led_state.h"
#include "sequential_leds_app/led_state.h"
//# Start User Includes Marker
#include "gpio/gpio.h"
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class led_controller : public Component
{
public:
  // Constructor
  led_controller(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Subscriber Callback - led_state_sub
  void led_state_sub_OnOneData(const sequential_leds_app::led_state::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~led_controller();

private:

  // Subscriber
  ros::Subscriber led_state_sub; 

  // Publisher 
  ros::Publisher led_state_pub;

  //# Start User Private Variables Marker
  bool ledState;
  bool invert;
  unsigned int ledPin;
  //# End User Private Variables Marker
};


#endif
