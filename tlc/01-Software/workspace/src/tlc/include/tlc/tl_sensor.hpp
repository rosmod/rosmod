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


#include "network/sender.hpp"

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

  // do we abide by the profiles?
  bool tg_misbehave;
  // size of messages generated
  uint64_t max_data_length;
  // Publisher 
  NAMESPACE::Publisher ryg_state_pub;
  // Timer for generating traffic
  NAMESPACE::Timer ryg_state_pub_timer;
  // Timer callback for traffic generation
  void ryg_state_pub_timerCallback(const NAMESPACE::TimerEvent& event);
  // publisher sender middleware
  Network::sender ryg_state_pub_send_mw;

  // Client 
  NAMESPACE::ServiceClient tlc_get_ryg_state_client;

  //# Start User Private Variables Marker
  std::string _id;
  std::string _last_state;
  //# End User Private Variables Marker
};

#endif
