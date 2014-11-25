#include "ros/ros.h"
#include "satellite_flight_application/SatelliteState.h"
#include "satellite_flight_application/SatelliteStateVector.h"
#include "satellite_flight_application/SatelliteThrusterControl.h"
#include "satellite_flight_application/TargetOrbit.h"
#include <cstdlib>

ros::NodeHandle n;

void targetOrbitCallback(const satellite_flight_application::TargetOrbit::ConstPtr& orbit)
{
  ROS_INFO("I got a new target orbit!");
  ROS_INFO("Activating the thrusters to achieve new orbit");
  ros::ServiceClient thrusterControlClient = n.serviceClient<satellite_flight_application::SatelliteThrusterControl>("SatelliteThrusterControl");

  satellite_flight_application::SatelliteThrusterControl srv;
  if (thrusterControlClient.call(srv))
    {
      ROS_INFO("Successfully activated satellite thrusters");
    }
  else
    {
      ROS_ERROR("Failed to activate satellite thrusters");
    }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "OrbitControllerActor");

  ros::ServiceClient satStateClient = n.serviceClient<satellite_flight_application::SatelliteStateVector>("SatelliteStateVector");

  ros::Publisher satStatePub = n.advertise<satellite_flight_application::SatelliteState>("SatelliteState", 1000);

  ros::Subscriber targetOrbitSub = n.subscribe("TargetOrbit", 1000, targetOrbitCallback);

  
  ros::Rate loop_rate(10);

  while (ros::ok())
    {
      satellite_flight_application::SatelliteStateVector srv;
      if (satStateClient.call(srv))
	{
	  ROS_INFO("Got state vector from satellite bus.");
	}
      else
	{
	  ROS_ERROR("Failed to get satellite state vector.");
	  return 1;
	}

      satellite_flight_application::SatelliteState satState;

      satStatePub.publish(satState);

      ros::spinOnce();

      loop_rate.sleep();
    }

  return 0;
}
