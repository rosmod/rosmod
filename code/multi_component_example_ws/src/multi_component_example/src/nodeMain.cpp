#include "ros/ros.h"
#include <cstdlib>

// required for boost::thread
#include <boost/thread.hpp>

// include all components this actor requires
#include "Component0.hpp"

std::string nodeName;

int main(int argc, char **argv)
{
  if ( argc != 2 )
    {
      ROS_INFO("usage: nodeMain <node name>");
      return 1;
    }
  nodeName = argv[1];
  // init handles ROS initialization
  ros::init(argc, argv, nodeName.c_str());
  // node handle must be instantiated after init!
  ros::NodeHandle n;

  Node1::Component0 comp0;

  // set up components here
  comp0.startUp();
  comp0.processQueue();

  return 0;
}
