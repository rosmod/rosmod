#ifndef CONTROLLER_1_HPP
#define CONTROLLER_1_HPP
#include "ros/ros.h"
#include "node/Component.hpp"
#include "dsc/ryg_control.h"
#include "dsc/ryg_state.h"
#include "dsc/sensor_state.h"
#include "dsc/sensor_state.h"
#include "dsc/sensor_state.h"
#include "dsc/sensor_state.h"
#include "dsc/sensor_state.h"
#include "dsc/sensor_state.h"
#include "dsc/sensor_state.h"
#include "dsc/sensor_state.h"



//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class controller_1 : public Component
{
public:
  // Constructor
  controller_1(ComponentConfig& _config, int argc, char **argv) : Component(_config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Subscriber Callback - ryg_state_sub
  void ryg_state_sub_OnOneData(const dsc::ryg_state::ConstPtr& received_data); 
 
  // Subscriber Callback - l1_ew_in
  void l1_ew_in_OnOneData(const dsc::sensor_state::ConstPtr& received_data); 
 
  // Subscriber Callback - l1_ew_out
  void l1_ew_out_OnOneData(const dsc::sensor_state::ConstPtr& received_data); 
 
  // Subscriber Callback - l2_ew_in
  void l2_ew_in_OnOneData(const dsc::sensor_state::ConstPtr& received_data); 
 
  // Subscriber Callback - l2_ew_out
  void l2_ew_out_OnOneData(const dsc::sensor_state::ConstPtr& received_data); 
 
  // Subscriber Callback - l1_ns_in
  void l1_ns_in_OnOneData(const dsc::sensor_state::ConstPtr& received_data); 
 
  // Subscriber Callback - l1_ns_out
  void l1_ns_out_OnOneData(const dsc::sensor_state::ConstPtr& received_data); 
 
  // Subscriber Callback - l2_ns_in
  void l2_ns_in_OnOneData(const dsc::sensor_state::ConstPtr& received_data); 
 
  // Subscriber Callback - l2_ns_out
  void l2_ns_out_OnOneData(const dsc::sensor_state::ConstPtr& received_data); 
 
  // Timer Callback - controller_timer
  void controller_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~controller_1();

private:

  // Timer
  ros::Timer controller_timer;

  // Subscriber
  ros::Subscriber ryg_state_sub;

  // Subscriber
  ros::Subscriber l1_ew_in;

  // Subscriber
  ros::Subscriber l1_ew_out;

  // Subscriber
  ros::Subscriber l2_ew_in;

  // Subscriber
  ros::Subscriber l2_ew_out;

  // Subscriber
  ros::Subscriber l1_ns_in;

  // Subscriber
  ros::Subscriber l1_ns_out;

  // Subscriber
  ros::Subscriber l2_ns_in;

  // Subscriber
  ros::Subscriber l2_ns_out;

  // Publisher 
  ros::Publisher ryg_control_pub;

  //# Start User Private Variables Marker
  std::string _id;
  int _clock[2]; // need to init
  int _queue[2]; // need to init
  int _Light_Min;// need to init
  int _Light_Max;// need to init
  int _s_NS;     // need to init
  int _s_WE;     // need to init
  std::string _state;
  int _total_latency;
  int _car_number;
  int _car_latency;
  int _truck_number;
  int _truck_latency;
  std::map<std::string,int> _sum_map;          // need to init
  std::map<std::string,int> _num_vehicles_map; // need to init
  std::map<std::string,std::string> _id_map;   // need to init
  std::map<std::string,std::vector<std::string>> _vehicle_ids_map; // need to init

  void vehicle_number( std::string sensor1,
		       std::string sensor2,
		       int& queue_length);
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
