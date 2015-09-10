#ifndef TL_SENSOR_HPP
#define TL_SENSOR_HPP
#include "ros/ros.h"
#include "node/Component.hpp"
#include "dsc/ryg_state.h"
#include "dsc/sumo_tlc_get_ryg_state.h"



//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class tl_sensor : public Component
{
public:
  // Constructor
  tl_sensor(ComponentConfig& _config, int argc, char **argv) : Component(_config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Timer Callback - tl_update_timer
  void tl_update_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~tl_sensor();

private:

  // Timer
  ros::Timer tl_update_timer;

  // Publisher 
  ros::Publisher ryg_state_pub;

  // Client 
  ros::ServiceClient tlc_get_ryg_state_client;

  //# Start User Private Variables Marker
  std::string _id;
  std::string _last_state;
  //# End User Private Variables Marker
};

#endif
