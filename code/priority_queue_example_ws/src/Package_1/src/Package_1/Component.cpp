#include "Package_1/Component.hpp"

// required for clean shutdown when process is killed
Component::~Component()
{
    cmq.disable();
    initOneShotTimer.stop();
}

void Component::Init(const ros::TimerEvent& event)
{
}

void Component::processQueue()
{  
    ros::NodeHandle2 nh;
    while (nh.ok())
    {
	this->cmq.callOne(ros::WallDuration(0.01));
    }
}
