#include "ros/ros.h"
#include "wam_application/HRImageVector.h"

#include <sstream>

std::string nodeName;

int main(int argc, char **argv)
{
  if ( argc != 2)
    {
      ROS_INFO("usage: HighResolution_a <satellite name>");
      return 1;
    }
  nodeName = argv[1];
  ros::init(argc, argv, "HighResolution_a");

  ros::NodeHandle n;

  ros::Publisher hrImage_pub = n.advertise<wam_application::HRImageVector>("HRImageVector", 1000);

  ros::Rate loop_rate(1);

  while (ros::ok())
    {
      wam_application::HRImageVector highResImgVec;

      highResImgVec.img[0] = 500;

      hrImage_pub.publish(highResImgVec);

      ROS_INFO("Published High Resolution Image from satellite %s", nodeName.c_str());

      ros::spinOnce();

      loop_rate.sleep();
    }


  return 0;
}
