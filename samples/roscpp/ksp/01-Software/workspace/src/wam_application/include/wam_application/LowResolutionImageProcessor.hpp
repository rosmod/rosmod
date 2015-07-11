#ifndef LOWRESOLUTIONIMAGEPROCESSOR_HPP
#define LOWRESOLUTIONIMAGEPROCESSOR_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "wam_application/LRImageVector.h"


//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class LowResolutionImageProcessor : public Component
{
public:
  // Constructor
  LowResolutionImageProcessor(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Timer Callback - LRImage_Timer
  void LRImage_TimerCallback(const ros::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~LowResolutionImageProcessor();

private:

  // Timer
  ros::Timer LRImage_Timer;

  // Publisher 
  ros::Publisher LRImage_Publisher;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
