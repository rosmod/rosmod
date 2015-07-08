#ifndef PID_HPP
#define PID_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "ksp_stearwing_controller/Actuation_Command.h"
#include "ksp_stearwing_controller/Sensor_Reading.h"
#include "ksp_stearwing_controller/Control_Command.h"


//# Start User Includes Marker
#include "ksp_stearwing_controller/libpid.hpp"
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class PID : public Component
{
public:
  // Constructor
  PID(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Subscriber Callback - sensor_subscriber
  void sensor_subscriber_OnOneData(const ksp_stearwing_controller::Sensor_Reading::ConstPtr& received_data); 
 
  // Subscriber Callback - pid_control_subscriber
  void pid_control_subscriber_OnOneData(const ksp_stearwing_controller::Control_Command::ConstPtr& received_data); 
 
  // Timer Callback - pid_timer
  void pid_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~PID();

private:

  // Timer
  ros::Timer pid_timer;

  // Subscriber
  ros::Subscriber sensor_subscriber; 

  // Subscriber
  ros::Subscriber pid_control_subscriber; 

  // Publisher 
  ros::Publisher actuator_control_publisher;

  //# Start User Private Variables Marker
                      float current_throttle;
  float current_pitch;
  float current_roll;
  float current_heading;
  double current_mean_altitude;
  double current_speed;

  float goal_heading;
  double goal_mean_altitude;
  double goal_speed;

  libpid mean_altitude_pid;
  libpid pitch_pid;
  libpid roll_pid;
  libpid heading_pid;
  libpid speed_pid;
  //# End User Private Variables Marker
};

#endif
