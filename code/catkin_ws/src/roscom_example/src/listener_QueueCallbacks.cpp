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

/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
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
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line. For programmatic
   * remappings you can use a different version of init() which takes remappings
   * directly, but for most command-line programs, passing argc and argv is the easiest
   * way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "listener_QueueCallbacks");
  // ros node names must be globally unique
  // unless the ros::init_options::AnonymousName is supplied

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle nh;
  
  /**
   * The subscribe() call is how you tell ROS that you want to receive messages
   * on a given topic.  This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing.  Messages are passed to a callback function, here
   * called chatterCallback.  subscribe() returns a Subscriber object that you
   * must hold on to until you want to unsubscribe.  When all copies of the Subscriber
   * object go out of scope, this callback will automatically be unsubscribed from
   * this topic.
   *
   * The second parameter to the subscribe() function is the size of the message
   * queue.  If messages are arriving faster than they are being processed, this
   * is the number of messages that will be buffered up before beginning to throw
   * away the oldest ones.
   */
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

  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
  //ros::spin();

  component0.join();
  component1.join();

  return 0;
}
