#ifndef ARM_CONTROLLER_HPP
#define ARM_CONTROLLER_HPP
#include "node/Component.hpp"
#include "agse_package/sampleState.h"
#include "agse_package/payloadBayState.h"
#include "agse_package/armState.h"
#include "agse_package/controlInputs.h"
#include "agse_package/sampleStateFromImage.h"
#include "agse_package/radialPos.h"
#include "agse_package/armRotation.h"
#include "agse_package/gripperRotation.h"
#include "agse_package/verticalPos.h"
#include "agse_package/gripperPos.h"
#include "agse_package/payloadBayStateFromImage.h"

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

class arm_controller : public Component
{
public:
  // Constructor
  arm_controller(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - controlInputs_sub
  void controlInputs_sub_operation(const agse_package::controlInputs::ConstPtr& received_data); 
 
  // Timer Operation - armTimer
  void armTimer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~arm_controller();

private:

  // Timer
  NAMESPACE::Timer armTimer;

  // Subscriber
  NAMESPACE::Subscriber controlInputs_sub;

  // Publisher 
  NAMESPACE::Publisher sampleState_pub;

  // Publisher 
  NAMESPACE::Publisher payloadBayState_pub;

  // Publisher 
  NAMESPACE::Publisher armState_pub;

  // Client 
  NAMESPACE::ServiceClient sampleStateFromImage_client;

  // Client 
  NAMESPACE::ServiceClient radialPos_client;

  // Client 
  NAMESPACE::ServiceClient armRotation_client;

  // Client 
  NAMESPACE::ServiceClient gripperRotation_client;

  // Client 
  NAMESPACE::ServiceClient verticalPos_client;

  // Client 
  NAMESPACE::ServiceClient gripperPos_client;

  // Client 
  NAMESPACE::ServiceClient payloadBayStateFromImage_client;

  //# Start User Private Variables Marker
  //# End User Private Variables Marker
};

#endif
