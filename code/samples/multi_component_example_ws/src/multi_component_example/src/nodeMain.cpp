#include "ros/ros.h"
#include <cstdlib>

// required for boost::thread
#include <boost/thread.hpp>

// include all components this actor requires
#include "Component0.hpp"
#include "Component1.hpp"
#include "Component2.hpp"

std::string nodeName;

void componentThread(Component* compPtr)
{
  compPtr->startUp();
  compPtr->processQueue();
}

int main(int argc, char **argv)
{
  if ( argc != 2 )
    {
      ROS_INFO("usage: nodeMain <node name>");
      return 1;
    }
  nodeName = argv[1];

  ros::init(argc, argv, nodeName.c_str());
  ros::NodeHandle n;

  Node1::Component0 comp0;
  Node1::Component1 comp1;
  Node1::Component2 comp2;

  boost::thread component0Thread(componentThread, &comp0);
  ROS_INFO("Node %s has started component0", nodeName.c_str());

  boost::thread component1Thread(componentThread, &comp1);
  ROS_INFO("Node %s has started component1", nodeName.c_str());

  boost::thread component2Thread(componentThread, &comp2);
  ROS_INFO("Node %s has started component2", nodeName.c_str());

  ROS_INFO_STREAM("NodeMain thread id=" << boost::this_thread::get_id());

  component0Thread.join();
  component1Thread.join();
  component2Thread.join();

  return 0;
}
