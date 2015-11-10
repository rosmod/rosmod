#ifndef HIGH_LEVEL_CONTROLLER_HPP
#define HIGH_LEVEL_CONTROLLER_HPP
#include "ros/ros.h"
#include "node/Component.hpp"
#include "rover_pkg/goal_state.h"
#include "rover_pkg/vessel_state.h"

#include "krpci/krpci.hpp"


//# Start User Includes Marker
#include <boost/circular_buffer.hpp>
//# End User Includes Marker

//# Start User Globals Marker
struct Waypoint {
  Waypoint() {}
  Waypoint(double latitude, 
	   double longitude,
	   double speed,
	   double lat_tolerance,
	   double long_tolerance) : 
    latitude_(latitude),
    longitude_(longitude),
    speed_(speed),
    lat_tolerance_(lat_tolerance),
    long_tolerance_(long_tolerance){}

  double latitude_;
  double longitude_;
  double speed_;
  double lat_tolerance_;
  double long_tolerance_;
};

struct Save_State {
  Save_State(float heading, 
	     double speed) : 
    heading_(heading),
    speed_(speed){}

  float heading_;
  double speed_;  
};
//# End User Globals Marker

class high_level_controller : public Component
{
public:
  // Constructor
  high_level_controller(ComponentConfig& _config, int argc, char **argv) : Component(_config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Subscriber Callback - vessel_state_sub
  void vessel_state_sub_OnOneData(const rover_pkg::vessel_state::ConstPtr& received_data); 
 
  // Timer Callback - state_timer
  void state_timerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~high_level_controller();

private:

  // Timer
  ros::Timer state_timer;

  // Subscriber
  ros::Subscriber vessel_state_sub;

  // Publisher 
  ros::Publisher goal_state_pub;

  //# Start User Private Variables Marker
    float current_heading;
  double current_latitude;
  double current_longitude;
  double current_speed;
  bool current_landing_gear;
  bool current_brakes;
  enum state { INIT, 
	       CRUISE };
  state current_state;

  float goal_heading;
  double goal_speed;

  float heading_tolerance;
  float speed_tolerance;
  double lat_tolerance;
  double long_tolerance;

  vector<Waypoint> cruise_waypoints;
  boost::circular_buffer<Save_State> previous_states;
  
  bool isGoalReached();
  bool state_func_INIT();
  bool state_func_CRUISE();
  //# End User Private Variables Marker
};

#endif
