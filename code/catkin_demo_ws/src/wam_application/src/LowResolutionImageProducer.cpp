#include "ros/ros.h"
#include "wma/LRImage.h"

#include <sstream>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "LR_Actor");

  ros::NodeHandle n;

  ros::Publisher lrImage_pub = n.advertise<wma::LRImage>("LRImage", 1000);

  ros::Rate loop_rate(10);

  while (ros::ok())
    {
      wma::LRImage img;

      img.LRImgVector.push_back(400);

      lrImage_pub.publish(img);

      ROS_INFO("Published Low Resolution Image");

      ros::spinOnce();

      loop_rate.sleep();
    }


  return 0;
}
