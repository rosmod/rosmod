#include "node/Component.hpp"

// Constructor
Component::Component(ComponentConfig &_config, int argc, char **argv) {
  logger.reset(new Logger());
  config = _config;
  node_argc = argc;
  node_argv = argv;
}

// Destructor
Component::~Component() {
  comp_queue.disable();
  init_timer.stop();
}

// Initialization
void Component::init_timer_operation(const NAMESPACE::TimerEvent& event) {}

// Synchronization
void Component::component_sync_operation(const std_msgs::Bool::ConstPtr& 
     received_data) {}

// Component Operation Queue Handler
void Component::process_queue() {  
  NAMESPACE::NodeHandle nh;
  while (nh.ok())
    this->comp_queue.callAvailable(ros::WallDuration(0.01));
}
