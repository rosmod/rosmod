#include "New_Package/Component.hpp"

// Component constructor - Obtaining node arguments
Component::Component(ComponentConfig &config, int argc, char **argv) {
  portGroupMap = config.portGroupMap;
  logLevels = config.logLevels;
  hostName = config.hostName;
  nodeName = config.nodeName;
  compName = config.compName;
  node_argc = argc;
  node_argv = argv;
}

// required for clean shutdown when process is killed
Component::~Component()
{
  compQueue.disable();
  initOneShotTimer.stop();
}

void Component::Init(const ros::TimerEvent& event)
{
}

void Component::processQueue()
{  
  ros::NodeHandle nh;
  while (nh.ok())
    {
      this->compQueue.callAvailable(ros::WallDuration(0.01));
    }
}
