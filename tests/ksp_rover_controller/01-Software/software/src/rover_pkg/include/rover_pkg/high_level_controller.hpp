#ifndef HIGH_LEVEL_CONTROLLER_HPP
#define HIGH_LEVEL_CONTROLLER_HPP
#include "node/Component.hpp"
#include "rover_pkg/goal_state.h"
#include "rover_pkg/vessel_state.h"

#ifdef USE_ROSMOD
  #include "rosmod/rosmod_ros.h"
#else
  #ifdef USE_ROSCPP
    #include "ros/ros.h"
  #endif
#endif

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
  high_level_controller(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - vessel_state_sub
  void vessel_state_sub_operation(const rover_pkg::vessel_state::ConstPtr& received_data); 
 
  // Timer Operation - state_timer
  void state_timer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~high_level_controller();

private:

  // Timer
  NAMESPACE::Timer state_timer;

  // Subscriber
  NAMESPACE::Subscriber vessel_state_sub;

  // Publisher 
  NAMESPACE::Publisher goal_state_pub;

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

  std::vector<Waypoint> cruise_waypoints;
  boost::circular_buffer<Save_State> previous_states;
  
  bool isGoalReached();
  bool state_func_INIT();
  bool state_func_CRUISE();
  //# End User Private Variables Marker
};

#endif
