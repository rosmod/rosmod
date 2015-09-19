#ifndef LINK_PROFILE_ENFORCER_HPP
#define LINK_PROFILE_ENFORCER_HPP
#include "ros/ros.h"
#include "node/Component.hpp"



//# Start User Includes Marker
#include "network/NetworkProfile.hpp"
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class link_profile_enforcer : public Component
{
public:
  // Constructor
  link_profile_enforcer(ComponentConfig& _config, int argc, char **argv) : Component(_config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~link_profile_enforcer();

private:

  //# Start User Private Variables Marker
  ros::Timer profile_timer;
  Network::NetworkProfile profile;
  std::string intf_name;
  void profile_timerCallback(const ros::TimerEvent& event);
  //# End User Private Variables Marker
};

#endif
