#ifndef USER_INPUT_CONTROLLER_HPP
#define USER_INPUT_CONTROLLER_HPP
#include "node/Component.hpp"
#include "agse_package/controlInputs.h"
#include "agse_package/armState.h"
#include "agse_package/sampleState.h"
#include "agse_package/payloadBayState.h"

#ifdef USE_ROSMOD
  #include "rosmod/rosmod_ros.h"
#else
  #ifdef USE_ROSCPP
    #include "ros/ros.h"
  #endif
#endif



//# Start User Includes Marker
#include "agse_package/gpio.h"
//#include "agse_package/uip.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"
using namespace cv;
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class user_input_controller : public Component
{
public:
  // Constructor
  user_input_controller(ComponentConfig& _config, int argc, char **argv)
  : Component(_config, argc, argv) {}

  // Initialization
  void init_timer_operation(const NAMESPACE::TimerEvent& event);

  // Subscriber Operation - armState_sub
  void armState_sub_operation(const agse_package::armState::ConstPtr& received_data); 
 
  // Subscriber Operation - sampleState_sub
  void sampleState_sub_operation(const agse_package::sampleState::ConstPtr& received_data); 
 
  // Subscriber Operation - payloadBayState_sub
  void payloadBayState_sub_operation(const agse_package::payloadBayState::ConstPtr& received_data); 
 
  // Timer Operation - userInputTimer
  void userInputTimer_operation(const NAMESPACE::TimerEvent& event);

  // Start up
  void startUp();

  // Destructor
  ~user_input_controller();

private:

  // Timer
  NAMESPACE::Timer userInputTimer;

  // Subscriber
  NAMESPACE::Subscriber armState_sub;

  // Subscriber
  NAMESPACE::Subscriber sampleState_sub;

  // Subscriber
  NAMESPACE::Subscriber payloadBayState_sub;

  // Publisher 
  NAMESPACE::Publisher controlInputs_pub;

  //# Start User Private Variables Marker
  bool paused;
  bool halted;
  bool manual;

  bool noGPIO;

  // used to keep track of AGSE state
  agse_package::armState arm;
  agse_package::sampleState sample;
  agse_package::payloadBayState payloadBay;

  // Pins for Pause (AMBER) Missile Switch
  unsigned int pauseSwitchPin; 
  // Pins for Manual Override (RED) Missile Switch
  unsigned int manualSwitchPin;
  // Pins for halt (BLUE) Missile Switch
  unsigned int haltSwitchPin; 

  // variable to keep track of switch states
  unsigned int pauseSwitchState;
  unsigned int haltSwitchState;
  unsigned int manualSwitchState;

  // Pin for Pause LED
  unsigned int pauseLED;
  unsigned int pauseLEDBlinkDelay;
  // Pin for Alarm LED
  unsigned int alarmLED;
  // Pin for Sample LED
  unsigned int sampleLED[3];
  unsigned int sampleLEDBlinkDelay;
  // Pin for Bay LED
  unsigned int bayLED[3];
  unsigned int bayLEDBlinkDelay;
  // Pin for Init LED
  unsigned int initLED[3];

  //# End User Private Variables Marker
};

#endif
