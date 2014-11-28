#include "ros/ros.h"
#include "wam_application/HRImageVector.h"
#include "wam_application/LRImageVector.h"

std::string nodeName;

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
  if ( argc != 2)
    {
      ROS_INFO("usage: ImageProcessor_a <satellite name>");
      return 1;
    }
  nodeName = argv[1];
  ros::init(argc, argv, "ImageProcessor_a");

  ros::NodeHandle n;

  ros::Subscriber HRsub = n.subscribe("HRImageVector", 1000, HRImgCallback);
  ros::Subscriber LRsub = n.subscribe("LRImageVector", 1000, LRImgCallback);

  ROS_INFO("Image Processor on satellite %s ready to receive images", nodeName.c_str());

  ros::spin();

  return 0;
}
