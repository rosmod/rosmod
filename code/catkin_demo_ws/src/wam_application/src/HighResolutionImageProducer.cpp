#include "ros/ros.h"
#include "wma/HRImage.h"

#include <sstream>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "HR_Actor");

  ros::NodeHandle n;

  ros::Publisher hrImage_pub = n.advertise<wma::HRImage>("HRImage", 1000);

  ros::Rate loop_rate(1);

  while (ros::ok())
    {
      wma::HRImage img;

      img.HRImgVector.push_back(500);

      hrImage_pub.publish(img);

      ROS_INFO("Published High Resolution Image");

      ros::spinOnce();

      loop_rate.sleep();
    }


  return 0;
}
