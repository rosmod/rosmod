#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP
#include "ros/ros.h"
#include "node/Component.hpp"
#include "dsc/ryg_control.h"
#include "dsc/sensor_state.h"
#include "dsc/ryg_state.h"



//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class controller : public Component
{
public:
  // Constructor
  controller(ComponentConfig& _config, int argc, char **argv) : Component(_config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Subscriber Callback - sensor_state_sub
  void sensor_state_sub_OnOneData(const dsc::sensor_state::ConstPtr& received_data); 
 
  // Subscriber Callback - ryg_state_sub
  void ryg_state_sub_OnOneData(const dsc::ryg_state::ConstPtr& received_data); 
 
  // Timer Callback - controller_timer
  void controller_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~controller();

private:

  // Timer
  ros::Timer controller_timer;

  // Subscriber
  ros::Subscriber sensor_state_sub;

  // Subscriber
  ros::Subscriber ryg_state_sub;

  // Publisher 
  ros::Publisher ryg_control_pub;

  //# Start User Private Variables Marker
  std::string _id;
  //# End User Private Variables Marker
};

#endif
