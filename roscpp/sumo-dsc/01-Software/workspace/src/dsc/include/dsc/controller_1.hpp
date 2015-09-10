#ifndef CONTROLLER_1_HPP
#define CONTROLLER_1_HPP
#include "ros/ros.h"
#include "node/Component.hpp"
#include "dsc/ryg_control.h"
#include "dsc/ryg_state.h"
#include "dsc/sensor_state.h"
#include "dsc/sensor_state.h"
#include "dsc/sensor_state.h"
#include "dsc/sensor_state.h"
#include "dsc/sensor_state.h"
#include "dsc/sensor_state.h"
#include "dsc/sensor_state.h"
#include "dsc/sensor_state.h"



//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class controller_1 : public Component
{
public:
  // Constructor
  controller_1(ComponentConfig& _config, int argc, char **argv) : Component(_config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Subscriber Callback - ryg_state_sub
  void ryg_state_sub_OnOneData(const dsc::ryg_state::ConstPtr& received_data); 
 
  // Subscriber Callback - l1_ew_in
  void l1_ew_in_OnOneData(const dsc::sensor_state::ConstPtr& received_data); 
 
  // Subscriber Callback - l1_ew_out
  void l1_ew_out_OnOneData(const dsc::sensor_state::ConstPtr& received_data); 
 
  // Subscriber Callback - l2_ew_in
  void l2_ew_in_OnOneData(const dsc::sensor_state::ConstPtr& received_data); 
 
  // Subscriber Callback - l2_ew_out
  void l2_ew_out_OnOneData(const dsc::sensor_state::ConstPtr& received_data); 
 
  // Subscriber Callback - l1_ns_in
  void l1_ns_in_OnOneData(const dsc::sensor_state::ConstPtr& received_data); 
 
  // Subscriber Callback - l1_ns_out
  void l1_ns_out_OnOneData(const dsc::sensor_state::ConstPtr& received_data); 
 
  // Subscriber Callback - l2_ns_in
  void l2_ns_in_OnOneData(const dsc::sensor_state::ConstPtr& received_data); 
 
  // Subscriber Callback - l2_ns_out
  void l2_ns_out_OnOneData(const dsc::sensor_state::ConstPtr& received_data); 
 
  // Timer Callback - controller_timer
  void controller_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~controller_1();

private:

  // Timer
  ros::Timer controller_timer;

  // Subscriber
  ros::Subscriber ryg_state_sub;

  // Subscriber
  ros::Subscriber l1_ew_in;

  // Subscriber
  ros::Subscriber l1_ew_out;

  // Subscriber
  ros::Subscriber l2_ew_in;

  // Subscriber
  ros::Subscriber l2_ew_out;

  // Subscriber
  ros::Subscriber l1_ns_in;

  // Subscriber
  ros::Subscriber l1_ns_out;

  // Subscriber
  ros::Subscriber l2_ns_in;

  // Subscriber
  ros::Subscriber l2_ns_out;

  // Publisher 
  ros::Publisher ryg_control_pub;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
