#include "ros/ros.h"
#include "satellite_flight_application/SatelliteCommand.h"

#include <sstream>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "talker");

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
