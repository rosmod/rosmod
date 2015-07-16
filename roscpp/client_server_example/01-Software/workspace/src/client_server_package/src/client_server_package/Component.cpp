#include "client_server_package/Component.hpp"

// Constructor
Component::Component(ComponentConfig &config, int argc, char **argv) {
  portGroupMap = config.portGroupMap;
  logLevels = config.logLevels;
  hostName = config.hostName;
  nodeName = config.nodeName;
  compName = config.compName;
  node_argc = argc;
  node_argv = argv;
  num_comps_to_sync = config.num_comps_to_sync;
  comp_sync_timeout = config.comp_sync_timeout;
}

// Destructor
Component::~Component() {
  compQueue.disable();
  initOneShotTimer.stop();
}

// Initialization
void Component::Init(const ros::TimerEvent& event) {}

// Synchronization
void Component::component_synchronization_OnOneData(const std_msgs::String::ConstPtr& received_data) {}

// Callback Queue Handler
void Component::processQueue() {  
  ros::NodeHandle nh;
  while (nh.ok()) {
    this->compQueue.callAvailable(ros::WallDuration(0.01));
  }
}
