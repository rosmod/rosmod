#include "node/Component.hpp"

// Constructor
Component::Component(ComponentConfig &config, int argc, char **argv) {
  portGroupMap = config.portGroupMap;
  logLevels = config.logLevels;
  hostName = config.hostName;
  nodeName = config.nodeName;
  compName = config.compName;
  node_argc = argc;
  node_argv = argv;
}

// Destructor
Component::~Component() {
  compQueue.disable();
  initOneShotTimer.stop();
}

// Initialization
void Component::Init(const ros::TimerEvent& event) {}

// Callback Queue Handler
void Component::processQueue() {  
  ros::NodeHandle nh;
  while (nh.ok()) {
    this->compQueue.callAvailable(ros::WallDuration(0.01));
  }
}
