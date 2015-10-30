#ifndef SUMO_INTF_HPP
#define SUMO_INTF_HPP
#include "node/Component.hpp"
#include "tlc/e3_get_vehicle_number.h"
#include "tlc/e3_get_vehicle_ids.h"
#include "tlc/tlc_get_ryg_state.h"
#include "tlc/tlc_set_ryg_state.h"

#ifdef USE_ROSMOD
  #include "rosmod/rosmod_ros.h"
#else
  #ifdef USE_ROSCPP
    #include "ros/ros.h"
  #endif
#endif

#include "sumo/sumo_client.hpp"


//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class sumo_intf : public Component
{
public:
  // Constructor
  sumo_intf(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Server Operation - e3_get_vehicle_number_server
  bool e3_get_vehicle_number_operation(tlc::e3_get_vehicle_number::Request &req, 
    tlc::e3_get_vehicle_number::Response &res);

  // Server Operation - e3_get_vehicle_ids_server
  bool e3_get_vehicle_ids_operation(tlc::e3_get_vehicle_ids::Request &req, 
    tlc::e3_get_vehicle_ids::Response &res);

  // Server Operation - tlc_get_ryg_state_server
  bool tlc_get_ryg_state_operation(tlc::tlc_get_ryg_state::Request &req, 
    tlc::tlc_get_ryg_state::Response &res);

  // Server Operation - tlc_set_ryg_state_server
  bool tlc_set_ryg_state_operation(tlc::tlc_set_ryg_state::Request &req, 
    tlc::tlc_set_ryg_state::Response &res);

  // Timer Operation - sumo_step_timer
  void sumo_step_timer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~sumo_intf();

private:

  // Timer
  NAMESPACE::Timer sumo_step_timer;

  // Server 
  NAMESPACE::ServiceServer e3_get_vehicle_number_server;

  // Server 
  NAMESPACE::ServiceServer e3_get_vehicle_ids_server;

  // Server 
  NAMESPACE::ServiceServer tlc_get_ryg_state_server;

  // Server 
  NAMESPACE::ServiceServer tlc_set_ryg_state_server;

  //# Start User Private Variables Marker
  std::map<std::string, int> _e3_num_vehicles_map;
  std::map<std::string, std::vector<std::string>> _e3_vehicle_ids_map;
  std::map<std::string, std::string> _tl_state_map;
  //# End User Private Variables Marker
};

#endif
