#ifndef TL_SENSOR_HPP
#define TL_SENSOR_HPP
#include "node/Component.hpp"
#include "tlc/ryg_state.h"
#include "tlc/tlc_get_ryg_state.h"

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

class tl_sensor : public Component
{
public:
  // Constructor
  tl_sensor(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Timer Operation - tl_update_timer
  void tl_update_timer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~tl_sensor();

private:

  // Timer
  NAMESPACE::Timer tl_update_timer;

  // Publisher 
  NAMESPACE::Publisher ryg_state_pub;

  // Client 
  NAMESPACE::ServiceClient tlc_get_ryg_state_client;

  //# Start User Private Variables Marker
  std::string _id;
  std::string _last_state;
  //# End User Private Variables Marker
};

#endif
