#ifndef USER_INPUT_IMAGER_HPP
#define USER_INPUT_IMAGER_HPP
#include "node/Component.hpp"
#include "agse_package/payloadBayDetectionImages.h"
#include "agse_package/sampleDetectionImages.h"
#include "agse_package/payloadBayState.h"
#include "agse_package/sampleState.h"
#include "agse_package/captureImage.h"

#ifdef USE_ROSMOD
  #include "rosmod/rosmod_ros.h"
#else
  #ifdef USE_ROSCPP
    #include "ros/ros.h"
  #endif
#endif

//# Start User Includes Marker
#include <opencv/cv.h>
#include <opencv/highgui.h>
using namespace cv;
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class user_input_imager : public Component
{
public:
  // Constructor
  user_input_imager(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - payloadBayDetectionImages_sub
  void payloadBayDetectionImages_sub_operation(const agse_package::payloadBayDetectionImages::ConstPtr& received_data); 
 
  // Subscriber Operation - sampleDetectionImages_sub
  void sampleDetectionImages_sub_operation(const agse_package::sampleDetectionImages::ConstPtr& received_data); 
 
  // Subscriber Operation - payloadBayState_sub
  void payloadBayState_sub_operation(const agse_package::payloadBayState::ConstPtr& received_data); 
 
  // Subscriber Operation - sampleState_sub
  void sampleState_sub_operation(const agse_package::sampleState::ConstPtr& received_data); 
 
  // Timer Operation - uiImage_timer
  void uiImage_timer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~user_input_imager();

private:

  // Timer
  NAMESPACE::Timer uiImage_timer;

  // Subscriber
  NAMESPACE::Subscriber payloadBayDetectionImages_sub;

  // Subscriber
  NAMESPACE::Subscriber sampleDetectionImages_sub;

  // Subscriber
  NAMESPACE::Subscriber payloadBayState_sub;

  // Subscriber
  NAMESPACE::Subscriber sampleState_sub;

  // Client 
  NAMESPACE::ServiceClient captureImage_client;

  //# Start User Private Variables Marker
  // Received Images
  Mat pb_rawImage;
  Mat pb_hsvImage;
  Mat pb_gsImage;
  Mat pb_bitwise;

  Mat sample_rawImage;
  Mat sample_hsvImage;
  Mat sample_gsImage;
  Mat sample_bitwise;

  // Four Images to show in UIP
  IplImage * top_left;
  IplImage * top_right;
  IplImage * bottom_left;
  IplImage * bottom_right;

  // Fullscreen single images
  IplImage * camera_feed;
  IplImage * processed_image;

  // Keyboard interrupt
  int key;
  //# End User Private Variables Marker
};

#endif
