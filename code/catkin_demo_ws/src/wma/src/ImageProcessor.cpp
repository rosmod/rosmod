#include "ros/ros.h"
#include "wma/HRImage.h"
#include "wma/LRImage.h"

void HRImgCallback(const wma::HRImage::ConstPtr& img)
{
  ROS_INFO("Got high-resolution image.");
}

void LRImgCallback(const wma::LRImage::ConstPtr& img)
{
  ROS_INFO("Got low-resolution image.");
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "Processor_Actor");

  ros::NodeHandle n;

  ros::Subscriber HRsub = n.subscribe("HRImage", 1000, HRImgCallback);
  ros::Subscriber LRsub = n.subscribe("LRImage", 1000, LRImgCallback);

  ROS_INFO("Image Processor ready to receive images");

  ros::spin();

  return 0;
}
