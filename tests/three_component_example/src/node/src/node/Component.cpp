#include "node/Component.hpp"

// Constructor
Component::Component(ComponentConfig &_config, int argc, char **argv) {
  config = _config;
  node_argc = argc;
  node_argv = argv;
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
