#ifndef HIGH_LEVEL_CONTROLLER_HPP
#define HIGH_LEVEL_CONTROLLER_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "flight_controller_package/Pitch.h"
#include "flight_controller_package/Roll.h"
#include "flight_controller_package/Heading.h"
#include "flight_controller_package/Altitude.h"


//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class High_level_Controller : public Component
{
public:
  // Constructor
  High_level_Controller(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Subscriber Callback - pitch_subscriber
  void pitch_subscriber_OnOneData(const flight_controller_package::Pitch::ConstPtr& received_data); 
 
  // Subscriber Callback - roll_subscriber
  void roll_subscriber_OnOneData(const flight_controller_package::Roll::ConstPtr& received_data); 
 
  // Subscriber Callback - heading_subscriber
  void heading_subscriber_OnOneData(const flight_controller_package::Heading::ConstPtr& received_data); 
 
  // Subscriber Callback - altitude_subscriber
  void altitude_subscriber_OnOneData(const flight_controller_package::Altitude::ConstPtr& received_data); 
 
  // Timer Callback - high_level_timer
  void high_level_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~High_level_Controller();

private:

  // Timer
  ros::Timer high_level_timer;

  // Subscriber
  ros::Subscriber pitch_subscriber; 

  // Subscriber
  ros::Subscriber roll_subscriber; 

  // Subscriber
  ros::Subscriber heading_subscriber; 

  // Subscriber
  ros::Subscriber altitude_subscriber; 

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
