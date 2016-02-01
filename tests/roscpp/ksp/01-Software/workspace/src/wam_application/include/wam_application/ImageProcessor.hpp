#ifndef IMAGEPROCESSOR_HPP
#define IMAGEPROCESSOR_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "wam_application/HRImageVector.h"
#include "wam_application/LRImageVector.h"


//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class ImageProcessor : public Component
{
public:
  // Constructor
  ImageProcessor(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Subscriber Callback - HRImage_Subscriber
  void HRImage_Subscriber_OnOneData(const wam_application::HRImageVector::ConstPtr& received_data); 
 
  // Subscriber Callback - LRImage_Subscriber
  void LRImage_Subscriber_OnOneData(const wam_application::LRImageVector::ConstPtr& received_data); 
 
  // Start up
  void startUp();

  // Destructor
  ~ImageProcessor();

private:

  // Subscriber
  ros::Subscriber HRImage_Subscriber; 

  // Subscriber
  ros::Subscriber LRImage_Subscriber; 

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
