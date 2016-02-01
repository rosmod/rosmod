#ifndef GROUNDINTERFACE_HPP
#define GROUNDINTERFACE_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "satellite_flight_application/GroundCommand.h"


//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class GroundInterface : public Component
{
public:
  // Constructor
  GroundInterface(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Timer Callback - GroundInterface_Timer
  void GroundInterface_TimerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~GroundInterface();

private:

  // Timer
  ros::Timer GroundInterface_Timer;

  // Publisher 
  ros::Publisher GroundCommand_Publisher;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
