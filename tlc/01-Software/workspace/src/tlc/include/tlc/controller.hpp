#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP
#include "node/Component.hpp"
#include "tlc/ryg_control.h"
#include "tlc/ryg_state.h"
#include "tlc/sensor_state.h"

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

class controller : public Component
{
public:
  // Constructor
  controller(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - ryg_state_sub
  void ryg_state_sub_operation(const tlc::ryg_state::ConstPtr& received_data); 
 
  // Subscriber Operation - sensor_state_sub
  void sensor_state_sub_operation(const tlc::sensor_state::ConstPtr& received_data); 
 
  // Timer Operation - controller_timer
  void controller_timer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~controller();

private:

  // Timer
  NAMESPACE::Timer controller_timer;

  // Subscriber
  NAMESPACE::Subscriber ryg_state_sub;

  // Subscriber
  NAMESPACE::Subscriber sensor_state_sub;

  // Publisher 
  NAMESPACE::Publisher ryg_control_pub;

  //# Start User Private Variables Marker
  // which traffic light are we controlling
  std::string _id;

  // map from sensor ID to sensor placement (e.g. "north")
  std::map<std::string,std::string> _sensor_id_map; // need to init
  
  // map from sensor placement to number of vehicles
  std::map<std::string,int> _num_vehicles_map; // need to init

  int _clock[2]; // need to init
  int _queue[2]; // need to init
  int _Light_Min;// need to init
  int _Light_Max;// need to init
  int _s_NS;     // need to init
  int _s_WE;     // need to init
  std::string _state;
  std::string _current_state;
  int _total_latency;
  int _car_number;
  int _car_latency;
  int _truck_number;
  int _truck_latency;

  void clock_value(const std::string& ns_state,
		   int& clock_WE,
		   int& clock_NS,
		   std::string& tl_state);
  void controller_main(std::string& tl_state,
		       const std::string& we_state,
		       const std::string& ns_state,
		       int queue_WE,
		       int queue_NS,
		       int& clock_WE,
		       int& clock_NS);
  //# End User Private Variables Marker
};

#endif
