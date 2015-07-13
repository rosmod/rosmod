#include "timer_package/Component.hpp"

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
void Component::Init(const rosmod::TimerEvent& event) {}

// Callback Queue Handler
void Component::processQueue() {  
  rosmod::NodeHandle nh;
  while (nh.ok()) {
    this->compQueue.callAvailable(rosmod::WallDuration(0.01));
  }
}
