#include "ros/ros.h"
#include "wam_application/HRImageVector.h"
#include "wam_application/LRImageVector.h"

void HRImgCallback(const wam_application::HRImageVector::ConstPtr& img)
{
  ROS_INFO("Got high-resolution image.");
}

void LRImgCallback(const wam_application::LRImageVector::ConstPtr& img)
{
  ROS_INFO("Got low-resolution image.");
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "ImageProcessor_a");

  ros::NodeHandle n;

  ros::Subscriber HRsub = n.subscribe("HRImageVector", 1000, HRImgCallback);
  ros::Subscriber LRsub = n.subscribe("LRImageVector", 1000, LRImgCallback);

  ROS_INFO("Image Processor ready to receive images");

  ros::spin();

  return 0;
}
