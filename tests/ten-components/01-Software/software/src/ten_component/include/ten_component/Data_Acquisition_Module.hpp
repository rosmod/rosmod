#ifndef DATA_ACQUISITION_MODULE_HPP
#define DATA_ACQUISITION_MODULE_HPP
#include "node/Component.hpp"
#include "ten_component/sensor_state.h"

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

class Data_Acquisition_Module : public Component
{
public:
  // Constructor
  Data_Acquisition_Module(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Timer Operation - sensor_read_timer
  void sensor_read_timer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~Data_Acquisition_Module();

private:

  // Timer
  NAMESPACE::Timer sensor_read_timer;

  // Publisher 
  NAMESPACE::Publisher sensor_state_publisher;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
