#include "Component0.hpp"

using namespace Node1;

void Component0::Init(const ros::TimerEvent& event)
{
  multi_component_example::ComponentName compName;
  compName.name = "Component0";
  ROS_INFO("Publishing component name %s",compName.name.c_str());
  compNamePub.publish(compName);
}

void Component0::OnOneData(const multi_component_example::ComponentName::ConstPtr& compName)
{
  ROS_INFO("Got component name %s",compName->name.c_str());
}

void Component0::Timer0Callback(const ros::TimerEvent& event)
{
  ROS_INFO("Timer0 callback has triggered!");
}

// EVERYTHING BELOW HERE IS COMPLETELY GENERATED

void Component0::startUp()
{
  ros::NodeHandle nh;

  // configure all subscribers associated with this component
  ros::SubscribeOptions queueSubOpts;

  queueSubOpts = 
    ros::SubscribeOptions::create<multi_component_example::ComponentName>
    ("ComponentName", 
     1000,
     boost::bind(&Component0::OnOneData, this, _1),
     ros::VoidPtr(),//(ros::VoidConstPtr)this,
     &this->compQueue);

  this->compNameSub = nh.subscribe(queueSubOpts);

  // configure all publishers associated with this component
  this->compNamePub = nh.advertise<multi_component_example::ComponentName>
    ("ComponentName", 1000);

  // configure all advertised services associated with this component

  // create the timers last to ensure we don't start off with too many callbacks in the queue
  ros::TimerOptions queueTimerOpts;

  // Need to run Init() once
  queueTimerOpts = ros::TimerOptions(
				     ros::Duration(-1), 
				     boost::bind(&Component0::Init, this, _1),
				     &this->compQueue,
				     true
				     );
  this->initOneShotTimer = nh.createTimer(queueTimerOpts);

  // need to start any timers this component contains
  queueTimerOpts = ros::TimerOptions(
				     ros::Duration(0.5),  // this value would be filled out by model
				     boost::bind(&Component0::Timer0Callback, this, _1),
				     &this->compQueue
				     );
  this->Timer0 = nh.createTimer(queueTimerOpts);
}

void Component0::processQueue()
{  
  ros::NodeHandle nh;
  while (nh.ok())
    {
      this->compQueue.callAvailable(ros::WallDuration(0.01));
    }
}
