#ifndef LOW_LEVEL_CONTROLLER_HPP
#define LOW_LEVEL_CONTROLLER_HPP
#include "node/Component.hpp"
#include "rover_pkg/control_command.h"
#include "rover_pkg/vessel_state.h"
#include "rover_pkg/goal_state.h"

#ifdef USE_ROSMOD
  #include "rosmod/rosmod_ros.h"
#else
  #ifdef USE_ROSCPP
    #include "ros/ros.h"
  #endif
#endif



//# Start User Includes Marker
#include "rover_pkg/libpid.hpp"
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class low_level_controller : public Component
{
public:
  // Constructor
  low_level_controller(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - vessel_state_sub
  void vessel_state_sub_operation(const rover_pkg::vessel_state::ConstPtr& received_data); 
 
  // Subscriber Operation - goal_state_sub
  void goal_state_sub_operation(const rover_pkg::goal_state::ConstPtr& received_data); 
 
  // Timer Operation - control_timer
  void control_timer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~low_level_controller();

private:

  // Timer
  NAMESPACE::Timer control_timer;

  // Subscriber
  NAMESPACE::Subscriber vessel_state_sub;

  // Subscriber
  NAMESPACE::Subscriber goal_state_sub;

  // Publisher 
  NAMESPACE::Publisher control_command_pub;

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
