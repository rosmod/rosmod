#ifndef PID_CONTROLLER_HPP
#define PID_CONTROLLER_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "flight_controller_package/Pitch.h"
#include "flight_controller_package/Roll.h"
#include "flight_controller_package/Heading.h"
#include "flight_controller_package/Altitude.h"
#include "flight_controller_package/Throttle_Control.h"
#include "flight_controller_package/Pitch_Control.h"
#include "flight_controller_package/Roll_Control.h"
#include "flight_controller_package/Heading_Control.h"
#include "flight_controller_package/Landing_Gear_Control.h"


//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class PID_Controller : public Component
{
public:
  // Constructor
  PID_Controller(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

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
 
  // Timer Callback - pid_control_timer
  void pid_control_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~PID_Controller();

private:

  // Timer
  ros::Timer pid_control_timer;

  // Subscriber
  ros::Subscriber pitch_subscriber; 

  // Subscriber
  ros::Subscriber roll_subscriber; 

  // Subscriber
  ros::Subscriber heading_subscriber; 

  // Subscriber
  ros::Subscriber altitude_subscriber; 

  // Client 
  ros::ServiceClient throttle_control_client;

  // Client 
  ros::ServiceClient pitch_control_client;

  // Client 
  ros::ServiceClient roll_control_client;

  // Client 
  ros::ServiceClient heading_control_client;

  // Client 
  ros::ServiceClient landing_gear_control_client;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
