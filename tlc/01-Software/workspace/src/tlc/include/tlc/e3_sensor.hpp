#ifndef E3_SENSOR_HPP
#define E3_SENSOR_HPP
#include "node/Component.hpp"
#include "tlc/sensor_state.h"
#include "tlc/e3_get_vehicle_number.h"

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

class e3_sensor : public Component
{
public:
  // Constructor
  e3_sensor(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Timer Operation - e3_update_timer
  void e3_update_timer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~e3_sensor();

private:

  // Timer
  NAMESPACE::Timer e3_update_timer;

  // do we abide by the profiles?
  bool tg_misbehave;
  // size of messages generated
  uint64_t max_data_length;
  // Publisher 
  NAMESPACE::Publisher sensor_state_pub;
  // Timer for generating traffic
  NAMESPACE::Timer sensor_state_pub_timer;
  // Timer callback for traffic generation
  void sensor_state_pub_timerCallback(const NAMESPACE::TimerEvent& event);
  // publisher sender middleware
  Network::sender sensor_state_pub_send_mw;

  // Client 
  NAMESPACE::ServiceClient e3_get_vehicle_number_client;

  //# Start User Private Variables Marker
  std::string _id;
  int _last_num_vehicles;
  std::vector<std::string> _last_vehicle_ids;
  //# End User Private Variables Marker
};

#endif
