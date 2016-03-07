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
#include "agse_package/detected_object.hpp"
#include "agse_package/SerialPort.h"
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
  bool paused;
  bool usingSerialPort;
  bool payloadBayOpened;
  bool stateChanged;
  enum ArmState
    {
      INIT = 0,
      OPENING_PB = 1,
      FINDING_SAMPLE = 2,
      FINDING_PB = 3,
      GRABBING_SAMPLE = 4,
      CARRYING_SAMPLE = 5,
      INSERTING_SAMPLE = 6,
      CLOSING_PB = 7,
      MOVING_AWAY = 8
    };

  void PrintCurrentState();
  void UpdateSensorData();
  void UpdateArmPosition();
  bool CheckGoals();

  ArmState currentState;
  agse_package::armState arm;
  agse_package::sampleState sample;
  agse_package::payloadBayState payloadBay;

  void Init_StateFunc();
  void Finding_PB_StateFunc();
  void Opening_PB_StateFunc();
  void Finding_Sample_StateFunc();
  void Grabbing_Sample_StateFunc();
  void Carrying_Sample_StateFunc();
  void Inserting_Sample_StateFunc();
  void Closing_PB_StateFunc();
  void Moving_Away_StateFunc();
  
  int maxRadialPos;
  int maxVerticalPos;
  float maxArmRotation;
  float maxGripperRotation;
  float maxGripperPos;

  int minRadialPos;
  int minVerticalPos;
  float minArmRotation;
  float minGripperRotation;
  float minGripperPos;

  float gripperRotationSafe;

  float gripperPosOpened;
  float gripperPosClosed;
  
  int currentRadialPos;
  int currentVerticalPos;
  float currentArmRotation;
  float currentGripperRotation;
  float currentGripperPos;

  int goalRadialPos;
  int goalVerticalPos;
  float goalArmRotation;
  float goalGripperRotation;
  float goalGripperPos;

  int radialEpsilon;
  int verticalEpsilon;
  float armRotationEpsilon;
  float gripperRotationEpsilon;
  float gripperPosEpsilon;

  int radialOffset;   // should be 0
  int verticalOffset; // should be 0
  float armRotationOffset; // difference between our 0 angle (arm frame) and servo's 0 angle
  float gripperRotationOffset; // difference between our 0 angle (plane of radius) and servo's 0 angle
  float gripperPosOffset; // difference between our 0 angle (open) and the servo's 0 angle

  float sampleOrientationOffset;  // difference between angle gotten from image and gripper angle

  // offsets measured between center of camera image and the center underneath the gripper (r,theta)
  int arcLengthBetweenGripperAndCamera;
  int radiusBetweenGripperAndCamera;

  // presets for the z-plane on which the sample and payload bay will lie (will be different)
  int sampleVerticalPos;
  int payloadBayVerticalPos;

  // serial port we use on the Jetson TK1 for reading from / writing to the payload bay
  SerialPort serialPort;
  char portName[50];

  //# End User Private Variables Marker
};

#endif
