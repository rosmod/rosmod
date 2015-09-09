#ifndef IL_SENSOR_HPP
#define IL_SENSOR_HPP
#include "ros/ros.h"
#include "node/Component.hpp"
#include "dsc/sensor_state.h"
#include "dsc/sumo_il_get_vehicle_ids.h"
#include "dsc/sumo_il_get_vehicle_number.h"

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

  // Publisher 
  ros::Publisher sensor_state_pub;

  // Client 
  ros::ServiceClient il_get_vehicle_ids_client;

  // Client 
  ros::ServiceClient il_get_vehicle_number_client;

  //# Start User Private Variables Marker
  std::string _id;
  int _last_num_vehicles;
  std::vector<std::string> _last_vehicle_ids;
  //# End User Private Variables Marker
};

#endif
