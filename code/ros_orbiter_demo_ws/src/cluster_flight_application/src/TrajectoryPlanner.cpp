#include "ros/ros.h"
#include "cluster_flight_application/SatState.h"
#include "cluster_flight_application/GroundCommand.h"
#include "cluster_flight_application/TargetOrbit.h"

ros::Publisher targetOrbitPub;
std::string nodeName;

std::vector<std::string> satNames;

void satCommandCallback(const cluster_flight_application::GroundCommand::ConstPtr& command)
{
  ROS_INFO("I got a satellite command!");
  ROS_INFO("Calculating new orbit");

  cluster_flight_application::TargetOrbit orbit;
  ROS_INFO("Publishing new target orbit");
  targetOrbitPub.publish(orbit);
}

void satStateCallback(const cluster_flight_application::SatState::ConstPtr& state)
{
  ROS_INFO("I got a satellite state from satellite %s",state->sat_id.c_str());
}

int main(int argc, char **argv)
{
  if ( argc != 2)
    {
      ROS_INFO("usage: TrajectoryPlanner_a <satellite name>");
      return 1;
    }
  nodeName = argv[1];
  ros::init(argc, argv, "TrajectoryPlanner_a");

  ros::NodeHandle n;

  ros::Subscriber satCommandSub = n.subscribe("GroundCommand", 1000, satCommandCallback);
  ros::Subscriber satStateSub = n.subscribe("SatState", 1000, satStateCallback);

  targetOrbitPub = n.advertise<cluster_flight_application::TargetOrbit>("TargetOrbit", 1000);

  ROS_INFO("Ready to receive commands and determine new orbits");

  ros::spin();

  return 0;
}
