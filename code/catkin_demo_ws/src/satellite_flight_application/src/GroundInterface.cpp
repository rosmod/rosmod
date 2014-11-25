#include "ros/ros.h"
#include "satellite_flight_application/SatelliteCommand.h"

#include <sstream>

int main(int argc, char **argv)
{
  if ( argc != 2)
    {
      ROS_INFO("usage: GndActor <satellite name>");
      return 1;
    }
  std::string nodeName = "GndActor";
  nodeName += argv[1];
  ros::init(argc, argv, nodeName.c_str());

  ros::NodeHandle n;

  ros::Publisher satCommandPub = n.advertise<satellite_flight_application::SatelliteCommand>("SatelliteCommand", 1000);

  ros::Rate loop_rate(10);

  while (ros::ok())
    {
      satellite_flight_application::SatelliteCommand satCommand;

      satCommandPub.publish(satCommand);

      ros::spinOnce();

      loop_rate.sleep();
    }

  return 0;
}
