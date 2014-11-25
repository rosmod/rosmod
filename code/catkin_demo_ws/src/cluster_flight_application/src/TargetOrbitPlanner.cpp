#include "ros/ros.h"
#include "satellite_flight_application/SatelliteState.h"
#include "satellite_flight_application/SatelliteCommand.h"
#include "cluster_flight_application/TargetOrbit.h"

void satCommandCallback(const satellite_flight_application::SatelliteCommand::ConstPtr& command)
{
  ROS_INFO("I got a satellite command!");
}

void satStateCallback(const satellite_flight_application::SatelliteState::ConstPtr& state)
{
  ROS_INFO("I got a satellite state!");
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "TargetOrbitActor");

  ros::NodeHandle n;

  ros::Subscriber satCommandSub = n.subscribe("SatelliteCommand", 1000, satCommandCallback);
  ros::Subscriber satStateSub = n.subscribe("SatelliteState", 1000, satStateCallback);

  ros::Publisher targetOrbitPub = n.advertise<cluster_flight_application::TargetOrbit>("TargetOrbit", 1000);

  ros::Rate loop_rate(10);

  while (ros::ok())
    {
      cluster_flight_application::TargetOrbit orbit;

      ROS_INFO("%s", msg.data.c_str());

      chatter_pub.publish(orbit);

      ros::spinOnce();

      loop_rate.sleep();
    }

  return 0;
}
