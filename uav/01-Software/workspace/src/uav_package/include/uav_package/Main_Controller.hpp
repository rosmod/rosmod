#ifndef MAIN_CONTROLLER_HPP
#define MAIN_CONTROLLER_HPP
#include "node/Component.hpp"
#include "uav_package/control_command.h"
#include "uav_package/sensor_state.h"

#ifdef USE_ROSMOD
  #include "rosmod/rosmod_ros.h"
#else
  #ifdef USE_ROSCPP
    #include "ros/ros.h"
  #endif
#endif



//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class Main_Controller : public Component
{
public:
  // Constructor
  Main_Controller(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - sensor_state_subscriber
  void sensor_state_subscriber_operation(const uav_package::sensor_state::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~Main_Controller();

private:

  // Subscriber
  NAMESPACE::Subscriber sensor_state_subscriber;

  // Publisher 
  NAMESPACE::Publisher control_command_publisher;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
