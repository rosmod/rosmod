#ifndef HIGHRESOLUTIONIMAGEPROCESSOR_HPP
#define HIGHRESOLUTIONIMAGEPROCESSOR_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "wam_application/HRImageVector.h"


//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class HighResolutionImageProcessor : public Component
{
public:
  // Constructor
  HighResolutionImageProcessor(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Timer Callback - HRImage_Timer
  void HRImage_TimerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~HighResolutionImageProcessor();

private:

  // Timer
  ros::Timer HRImage_Timer;

  // Publisher 
  ros::Publisher HRImage_Publisher;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
