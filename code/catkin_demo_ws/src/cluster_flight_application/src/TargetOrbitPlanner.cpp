#include "ros/ros.h"
#include "cluster_flight_application/SatelliteState.h"
#include "cluster_flight_application/SatelliteCommand.h"
#include "cluster_flight_application/TargetOrbit.h"

ros::Publisher targetOrbitPub;

void satCommandCallback(const cluster_flight_application::SatelliteCommand::ConstPtr& command)
{
  ROS_INFO("I got a satellite command!");
  ROS_INFO("Calculating new orbit");
  ROS_INFO("Publishing new target orbit");

  cluster_flight_application::TargetOrbit orbit;
  targetOrbitPub.publish(orbit);
}

void satStateCallback(const cluster_flight_application::SatelliteState::ConstPtr& state)
{
  ROS_INFO("I got a satellite state!");
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "TargetOrbitActor");

  ros::NodeHandle n;

  ros::Subscriber satCommandSub = n.subscribe("SatelliteCommand", 1000, satCommandCallback);
  ros::Subscriber satStateSub = n.subscribe("SatelliteState", 1000, satStateCallback);

  targetOrbitPub = n.advertise<cluster_flight_application::TargetOrbit>("TargetOrbit", 1000);

  ros::spin();

  return 0;
}
