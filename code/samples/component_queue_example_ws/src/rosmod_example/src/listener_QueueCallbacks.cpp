#include "ros/ros.h"
#include "std_msgs/String.h"

// required for the callback queue
#include <ros/callback_queue.h>
// required to provide callback queue to subscribe function
#include <ros/subscribe_options.h>
// required for boost::thread
#include <boost/thread.hpp>

ros::CallbackQueue component0Queue;
ros::CallbackQueue component1Queue;
ros::SubscribeOptions componentQueueSubscribeOptions;

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO_STREAM(
		  "I heard: [" << 
		  msg->data << 
		  "] in thread [" << 
		  boost::this_thread::get_id() << 
		  "]");
}

// This callback is where the user would (like in DREMS)
// write their business logic for handling one incoming message
void OnOneData(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO_STREAM(
		  "OnOneData: I heard: [" << 
		  msg->data << 
		  "] in thread [" << 
		  boost::this_thread::get_id() << 
		  "]");
}

void componentThread(ros::CallbackQueue* componentQueue)
{
  // could call ros::init() here to start a new node

  ROS_INFO_STREAM("Callback thread ID=" << boost::this_thread::get_id());
  
  ros::NodeHandle nh;
  while (nh.ok())
    {
      componentQueue->callAvailable(ros::WallDuration(0.01));
    }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener_QueueCallbacks");
  // ros node names must be globally unique
  // unless the ros::init_options::AnonymousName is supplied

  ros::NodeHandle nh;
  
  // sub 0 will run in this thread (the main thread)
  ros::Subscriber sub0 = nh.subscribe("chatter", 1000, chatterCallback);

  // configure subs 1,2,3 callbacks to use component 0's queue
  componentQueueSubscribeOptions = 
    ros::SubscribeOptions::create<std_msgs::String>("chatter", 1000,
						    OnOneData,
						    ros::VoidPtr(), &component0Queue);
  // subs 1,2,3 will run in the componentQueueThread
  ros::Subscriber sub1 = nh.subscribe(componentQueueSubscribeOptions);
  ros::Subscriber sub2 = nh.subscribe(componentQueueSubscribeOptions);
  ros::Subscriber sub3 = nh.subscribe(componentQueueSubscribeOptions);

  // configure subs 4,5 callbacks to use component 1's queue
  componentQueueSubscribeOptions = 
    ros::SubscribeOptions::create<std_msgs::String>("chatter", 1000,
						    OnOneData,
						    ros::VoidPtr(), &component1Queue);
  ros::Subscriber sub4 = nh.subscribe(componentQueueSubscribeOptions);
  ros::Subscriber sub5 = nh.subscribe(componentQueueSubscribeOptions);

  boost::thread component0(componentThread, &component0Queue);
  boost::thread component1(componentThread, &component1Queue);

  ROS_INFO_STREAM("Main thread id=" << boost::this_thread::get_id());

  component0.join();
  component1.join();

  return 0;
}
