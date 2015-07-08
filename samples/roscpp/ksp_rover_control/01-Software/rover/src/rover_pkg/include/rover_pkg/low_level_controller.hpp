#ifndef LOW_LEVEL_CONTROLLER_HPP
#define LOW_LEVEL_CONTROLLER_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "rover_pkg/control_command.h"
#include "rover_pkg/vessel_state.h"
#include "rover_pkg/goal_state.h"


//# Start User Includes Marker
#include "rover_pkg/libpid.hpp"
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class low_level_controller : public Component
{
public:
  // Constructor
  low_level_controller(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Subscriber Callback - vessel_state_sub
  void vessel_state_sub_OnOneData(const rover_pkg::vessel_state::ConstPtr& received_data); 
 
  // Subscriber Callback - goal_state_sub
  void goal_state_sub_OnOneData(const rover_pkg::goal_state::ConstPtr& received_data); 
 
  // Timer Callback - control_timer
  void control_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~low_level_controller();

private:

  // Timer
  ros::Timer control_timer;

  // Subscriber
  ros::Subscriber vessel_state_sub; 

  // Subscriber
  ros::Subscriber goal_state_sub; 

  // Publisher 
  ros::Publisher control_command_pub;

  //# Start User Private Variables Marker
    float current_heading;
  float current_speed;

  float goal_heading;
  double goal_speed;

  libpid heading_pid;
  libpid speed_pid;
  //# End User Private Variables Marker
};

#endif
