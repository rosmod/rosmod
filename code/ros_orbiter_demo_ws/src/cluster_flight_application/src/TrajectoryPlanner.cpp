#include "ros/ros.h"
#include "cluster_flight_application/SatState.h"
#include "cluster_flight_application/GroundCommand.h"
#include "cluster_flight_application/TargetOrbit.h"

ros::Publisher targetOrbitPub;

void satCommandCallback(const cluster_flight_application::GroundCommand::ConstPtr& command)
{
  ROS_INFO("I got a satellite command!");
  ROS_INFO("Calculating new orbit");
  ROS_INFO("Publishing new target orbit");

  cluster_flight_application::TargetOrbit orbit;
  targetOrbitPub.publish(orbit);
}

void satStateCallback(const cluster_flight_application::SatState::ConstPtr& state)
{
  ROS_INFO("I got a satellite state!");
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "TrajectoryPlanner_a");

  ros::NodeHandle n;

  ros::Subscriber satCommandSub = n.subscribe("GroundCommand", 1000, satCommandCallback);
  ros::Subscriber satStateSub = n.subscribe("SatState", 1000, satStateCallback);

  targetOrbitPub = n.advertise<cluster_flight_application::TargetOrbit>("TargetOrbit", 1000);

  ROS_INFO("Ready to receive commands and determine new orbits");

  ros::spin();

  return 0;
}
