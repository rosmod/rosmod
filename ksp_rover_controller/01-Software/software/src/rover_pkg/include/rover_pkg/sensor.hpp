#ifndef SENSOR_HPP
#define SENSOR_HPP
#include "node/Component.hpp"
#include "rover_pkg/vessel_state.h"

#ifdef USE_ROSMOD
  #include "rosmod/rosmod_ros.h"
#else
  #ifdef USE_ROSCPP
    #include "ros/ros.h"
  #endif
#endif

#include "krpci/krpci.hpp"


//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class sensor : public Component
{
public:
  // Constructor
  sensor(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Timer Operation - sensor_timer
  void sensor_timer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~sensor();

private:

  // Timer
  NAMESPACE::Timer sensor_timer;

  // Publisher 
  NAMESPACE::Publisher vessel_state_pub;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
