#ifndef SUMO_INTF_HPP
#define SUMO_INTF_HPP
#include "ros/ros.h"
#include "node/Component.hpp"
#include "dsc/sumo_il_get_vehicle_number.h"
#include "dsc/sumo_il_get_vehicle_ids.h"
#include "dsc/sumo_tlc_get_ryg_state.h"
#include "dsc/sumo_tlc_set_ryg_state.h"



//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class sumo_intf : public Component
{
public:
  // Constructor
  sumo_intf(ComponentConfig& _config, int argc, char **argv) : Component(_config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Server Callback - sumo_il_get_vehicle_number_server
  bool sumo_il_get_vehicle_numberCallback(dsc::sumo_il_get_vehicle_number::Request &req, 
    dsc::sumo_il_get_vehicle_number::Response &res);

  // Server Callback - sumo_il_get_vehicle_ids_server
  bool sumo_il_get_vehicle_idsCallback(dsc::sumo_il_get_vehicle_ids::Request &req, 
    dsc::sumo_il_get_vehicle_ids::Response &res);

  // Server Callback - sumo_tlc_get_ryg_state_server
  bool sumo_tlc_get_ryg_stateCallback(dsc::sumo_tlc_get_ryg_state::Request &req, 
    dsc::sumo_tlc_get_ryg_state::Response &res);

  // Server Callback - sumo_tlc_set_ryg_state_server
  bool sumo_tlc_set_ryg_stateCallback(dsc::sumo_tlc_set_ryg_state::Request &req, 
    dsc::sumo_tlc_set_ryg_state::Response &res);

  // Timer Callback - sumo_step_timer
  void sumo_step_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~sumo_intf();

private:

  // Timer
  ros::Timer sumo_step_timer;

  // Server 
  ros::ServiceServer sumo_il_get_vehicle_number_server;

  // Server 
  ros::ServiceServer sumo_il_get_vehicle_ids_server;

  // Server 
  ros::ServiceServer sumo_tlc_get_ryg_state_server;

  // Server 
  ros::ServiceServer sumo_tlc_set_ryg_state_server;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
