#ifndef IMAGE_PROCESSOR_HPP
#define IMAGE_PROCESSOR_HPP
#include "node/Component.hpp"
#include "agse_package/payloadBayDetectionImages.h"
#include "agse_package/sampleDetectionImages.h"
#include "agse_package/controlInputs.h"
#include "agse_package/captureImage.h"
#include "agse_package/sampleStateFromImage.h"
#include "agse_package/payloadBayStateFromImage.h"

#ifdef USE_ROSMOD
  #include "rosmod/rosmod_ros.h"
#else
  #ifdef USE_ROSCPP
    #include "ros/ros.h"
  #endif
#endif



//# Start User Includes Marker
#include "agse_package/sample_detector.hpp"
#include "agse_package/payloadbay_detector.hpp"
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class image_processor : public Component
{
public:
  // Constructor
  image_processor(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - controlInputs_sub
  void controlInputs_sub_operation(const agse_package::controlInputs::ConstPtr& received_data); 
 
  // Server Operation - sampleStateFromImage_server
  bool sampleStateFromImage_operation(agse_package::sampleStateFromImage::Request &req, 
    agse_package::sampleStateFromImage::Response &res);

  // Server Operation - payloadBayStateFromImage_server
  bool payloadBayStateFromImage_operation(agse_package::payloadBayStateFromImage::Request &req, 
    agse_package::payloadBayStateFromImage::Response &res);

  // Start up
  void startUp();

  // Destructor
  ~image_processor();

private:

  // Subscriber
  NAMESPACE::Subscriber controlInputs_sub;

  // Publisher 
  NAMESPACE::Publisher payloadBayDetectionImages_pub;

  // Publisher 
  NAMESPACE::Publisher sampleDetectionImages_pub;

  // Server 
  NAMESPACE::ServiceServer sampleStateFromImage_server;

  // Server 
  NAMESPACE::ServiceServer payloadBayStateFromImage_server;

  // Client 
  NAMESPACE::ServiceClient captureImage_client;

  //# Start User Private Variables Marker
  bool paused;
  Sample_Detector sampleDetector;
  PayloadBay_Detector payloadBayDetector;
  //# End User Private Variables Marker
};

#endif
