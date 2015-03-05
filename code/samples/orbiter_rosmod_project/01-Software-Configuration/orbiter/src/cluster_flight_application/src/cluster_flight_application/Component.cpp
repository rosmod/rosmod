#include "cluster_flight_application/Component.hpp"

// Component constructor - Obtaining node arguments
Component::Component(std::string node, std::string comp, int argc, char **argv) {
    nodeName = node;
    compName = comp;
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
