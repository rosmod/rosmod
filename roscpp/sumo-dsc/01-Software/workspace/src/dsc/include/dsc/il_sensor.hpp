#ifndef IL_SENSOR_HPP
#define IL_SENSOR_HPP
#include "ros/ros.h"
#include "node/Component.hpp"
#include "dsc/sensor_state.h"
#include "dsc/sumo_il_get_vehicle_ids.h"
#include "dsc/sumo_il_get_vehicle_number.h"


#include "network/sender.hpp"

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class il_sensor : public Component
{
public:
  // Constructor
  il_sensor(ComponentConfig& _config, int argc, char **argv) : Component(_config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Timer Callback - il_update_timer
  void il_update_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~il_sensor();

private:

  // Timer
  ros::Timer il_update_timer;

  // do we abide by the profiles?
  bool tg_misbehave;
  // size of messages generated
  uint64_t max_data_length;
  // Publisher 
  ros::Publisher sensor_state_pub;
  // Timer for generating traffic
  ros::Timer sensor_state_pub_timer;
  // Timer callback for traffic generation
  void sensor_state_pub_timerCallback(const ros::TimerEvent& event);
  // publisher sender middleware
  Network::sender sensor_state_pub_send_mw;

  // Client 
  ros::ServiceClient il_get_vehicle_ids_client;

  // Client 
  ros::ServiceClient il_get_vehicle_number_client;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
