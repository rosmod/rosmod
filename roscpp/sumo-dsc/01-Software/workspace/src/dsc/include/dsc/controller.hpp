#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP
#include "ros/ros.h"
#include "node/Component.hpp"
#include "dsc/ryg_control.h"
#include "dsc/sensor_state.h"
#include "dsc/ryg_state.h"


#include "network/sender.hpp"
#include "network/receiver.hpp"

//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class controller : public Component
{
public:
  // Constructor
  controller(ComponentConfig& _config, int argc, char **argv) : Component(_config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Subscriber Callback - sensor_state_sub
  void sensor_state_sub_OnOneData(const dsc::sensor_state::ConstPtr& received_data); 
 
  // Subscriber Callback - ryg_state_sub
  void ryg_state_sub_OnOneData(const dsc::ryg_state::ConstPtr& received_data); 
 
  // Timer Callback - controller_timer
  void controller_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~controller();

private:

  // Timer
  ros::Timer controller_timer;

  // callback func for when servers are done receiving data
  void mw_recv_done_callback(Network::receiver* receiver_mw);
  // Subscriber
  ros::Subscriber sensor_state_sub;
  // message id for this data stream
  uint64_t sensor_state_sub_id;
  // subscriber receiver middleware
  Network::receiver sensor_state_sub_recv_mw;

  // Subscriber
  ros::Subscriber ryg_state_sub;
  // message id for this data stream
  uint64_t ryg_state_sub_id;
  // subscriber receiver middleware
  Network::receiver ryg_state_sub_recv_mw;

  // do we abide by the profiles?
  bool tg_misbehave;
  // size of messages generated
  uint64_t max_data_length;
  // Publisher 
  ros::Publisher ryg_control_pub;
  // Timer for generating traffic
  ros::Timer ryg_control_pub_timer;
  // Timer callback for traffic generation
  void ryg_control_pub_timerCallback(const ros::TimerEvent& event);
  // publisher sender middleware
  Network::sender ryg_control_pub_send_mw;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
