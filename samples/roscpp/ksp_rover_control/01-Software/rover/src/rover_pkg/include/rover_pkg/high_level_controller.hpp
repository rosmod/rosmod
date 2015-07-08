#ifndef HIGH_LEVEL_CONTROLLER_HPP
#define HIGH_LEVEL_CONTROLLER_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "rover_pkg/goal_state.h"
#include "rover_pkg/vessel_state.h"


//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class high_level_controller : public Component
{
public:
  // Constructor
  high_level_controller(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Subscriber Callback - vessel_state_sub
  void vessel_state_sub_OnOneData(const rover_pkg::vessel_state::ConstPtr& received_data); 
 
  // Timer Callback - state_timer
  void state_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~high_level_controller();

private:

  // Timer
  ros::Timer state_timer;

  // Subscriber
  ros::Subscriber vessel_state_sub; 

  // Publisher 
  ros::Publisher goal_state_pub;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
