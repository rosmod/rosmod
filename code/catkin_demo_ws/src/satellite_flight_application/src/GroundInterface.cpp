#include "ros/ros.h"
#include "satellite_flight_application/SatelliteCommand.h"

#include <sstream>

int main(int argc, char **argv)
{
  if ( argc != 3)
    {
      ROS_INFO("usage: GndActor <satellite name> <0,1 to disable/enable publishing>");
      return 1;
    }
  std::string nodeName = "GndActor";
  nodeName += argv[1];
  ros::init(argc, argv, nodeName.c_str());

  ros::NodeHandle n;

  ros::Publisher satCommandPub = n.advertise<satellite_flight_application::SatelliteCommand>("SatelliteCommand", 1000);

  ros::Rate loop_rate(1);

  bool publishCommands = atol(argv[2]);

  if (publishCommands)
    ROS_INFO("%s will publish commands",nodeName.c_str());
  else
    ROS_INFO("%s will not publish commands",nodeName.c_str());

  while (ros::ok())
    {
      if (publishCommands)
	{
	  satellite_flight_application::SatelliteCommand satCommand;
	  satCommandPub.publish(satCommand);
	  ROS_INFO("Published new satCommand");
	}

      ros::spinOnce();

      loop_rate.sleep();
    }

  return 0;
}
