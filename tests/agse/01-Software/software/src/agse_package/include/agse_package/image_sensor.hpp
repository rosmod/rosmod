#ifndef IMAGE_SENSOR_HPP
#define IMAGE_SENSOR_HPP
#include "node/Component.hpp"
#include "agse_package/controlInputs.h"
#include "agse_package/captureImage.h"

#ifdef USE_ROSMOD
  #include "rosmod/rosmod_ros.h"
#else
  #ifdef USE_ROSCPP
    #include "ros/ros.h"
  #endif
#endif



//# Start User Includes Marker
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class image_sensor : public Component
{
public:
  // Constructor
  image_sensor(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - controlInputs_sub
  void controlInputs_sub_operation(const agse_package::controlInputs::ConstPtr& received_data); 
 
  // Server Operation - captureImage_server
  bool captureImage_operation(agse_package::captureImage::Request &req, 
    agse_package::captureImage::Response &res);

  // Start up
  void startUp();

  // Destructor
  ~image_sensor();

private:

  // Subscriber
  NAMESPACE::Subscriber controlInputs_sub;

  // Server 
  NAMESPACE::ServiceServer captureImage_server;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
