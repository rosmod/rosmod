#include "node/Component.hpp"

// Constructor
Component::Component(ComponentConfig &_config, int argc, char **argv) {
  this->config = _config;
  node_argc = argc;
  node_argv = argv;
  /* MOVE TO COMPONENT INSTANCE STARTUP
  if (config.schedulingScheme == "FIFO")
    scheduling_scheme = rosmod::CallbackQueue::SchedulingScheme::FIFO;
  else if (config.schedulingScheme == "PFIFO")
    scheduling_scheme = rosmod::CallbackQueue::SchedulingScheme::PFIFO;
  else if (config.schedulingScheme == "EDF")
    scheduling_scheme = rosmod::CallbackQueue::SchedulingScheme::EDF;
  */
}

// Destructor
Component::~Component() {
  compQueue.disable();
  initOneShotTimer.stop();
}

// Initialization
void Component::Init(const NAMESPACE::TimerEvent& event) {}

// Synchronization
void Component::component_synchronization_OnOneData(const std_msgs::Bool::ConstPtr& received_data) {}

// Callback Queue Handler
void Component::processQueue() {  
  NAMESPACE::NodeHandle nh;
  while (nh.ok()) {
    this->compQueue.callAvailable(ros::WallDuration(0.01));
  }
}
