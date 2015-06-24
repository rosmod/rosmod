#ifndef HIGH_LEVEL_CONTROLLER_HPP
#define HIGH_LEVEL_CONTROLLER_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "ksp_stearwing_controller/Control_Command.h"
#include "ksp_stearwing_controller/Sensor_Reading.h"

#include "krpci/krpci.hpp"

//# Start User Includes Marker
#include <boost/circular_buffer.hpp>
//# End User Includes Marker

//# Start User Globals Marker
class Waypoint {
public:
  Waypoint(double altitude, 
	   double latitude, 
	   double longitude) : 
    altitude_(altitude),
    latitude_(latitude),
    longitude_(longitude){}

  double altitude_;
  double latitude_;
  double longitude_;
};

class Save_State {
public:
  Save_State(float heading, 
	     double altitude, 
	     double speed) : 
    heading_(heading),
    altitude_(altitude),
    speed_(speed){}

  float heading_;
  double altitude_;
  double speed_;  
};
//# End User Globals Marker

class High_level_Controller : public Component
{
public:
  // Constructor
  High_level_Controller(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Subscriber Callback - sensor_subscriber
  void sensor_subscriber_OnOneData(const ksp_stearwing_controller::Sensor_Reading::ConstPtr& received_data); 
 
  // Timer Callback - flight_control_timer
  void flight_control_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~High_level_Controller();

private:

  // Timer
  ros::Timer flight_control_timer;

  // Subscriber
  ros::Subscriber sensor_subscriber; 

  // Publisher 
  ros::Publisher pid_control_publisher;

  //# Start User Private Variables Marker
  float current_throttle;
  float current_pitch;
  float current_roll;
  float current_heading;
  double current_altitude;
  double current_latitude;
  double current_longitude;
  double current_speed;
  enum state { INIT, 
	       TAKE_OFF, 
	       CRUISE, 
	       LAND };
  state current_state;

  float goal_heading;
  double goal_altitude;
  double goal_speed;

  float heading_tolerance;
  float altitude_tolerance;
  float speed_tolerance;

  vector<Waypoint> cruise_waypoints;
  boost::circular_buffer<Save_State> previous_states;
  
  bool isGoalReached();
  bool state_func_INIT();
  bool state_func_TAKEOFF();
  bool state_func_CRUISE();
  bool state_func_LAND();
  //# End User Private Variables Marker
};

#endif
