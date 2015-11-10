#include "node/Component.hpp"

// Constructor
Component::Component(ComponentConfig &_config, int argc, char **argv) {
  this->config = _config;
  portGroupMap = config.portGroupMap;
  logLevels = config.logLevels;
  hostName = config.hostName;
  nodeName = config.nodeName;
  compName = config.compName;
  is_periodic_logging = config.is_periodic_logging;
  periodic_log_unit = config.periodic_log_unit;
  node_argc = argc;
  node_argv = argv;
  num_comps_to_sync = config.num_comps_to_sync;
  comp_sync_timeout = config.comp_sync_timeout;
  if (config.schedulingScheme == "FIFO")
    scheduling_scheme = rosmod::CallbackQueue::SchedulingScheme::FIFO;
  else if (config.schedulingScheme == "PFIFO")
    scheduling_scheme = rosmod::CallbackQueue::SchedulingScheme::PFIFO;
  else if (config.schedulingScheme == "EDF")
    scheduling_scheme = rosmod::CallbackQueue::SchedulingScheme::EDF;
}

// Destructor
Component::~Component() {
  compQueue.disable();
  initOneShotTimer.stop();
}

// Initialization
void Component::Init(const rosmod::TimerEvent& event) {}

// Synchronization
void Component::component_synchronization_OnOneData(const std_msgs::Bool::ConstPtr& received_data) {}

// Callback Queue Handler
void Component::processQueue() {  
  rosmod::NodeHandle nh;
  while (nh.ok()) {
    this->compQueue.callAvailable(ros::WallDuration(0.01));
  }
}
