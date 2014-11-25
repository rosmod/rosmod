#include "ros/ros.h"
#include "satellite_flight_application/SatelliteState.h"
#include "satellite_flight_application/SatelliteStateVector.h"
#include "satellite_flight_application/SatelliteThrusterControl.h"
#include "satellite_flight_application/TargetOrbit.h"
#include <cstdlib>

ros::ServiceClient thrusterControlClient;

void targetOrbitCallback(const satellite_flight_application::TargetOrbit::ConstPtr& orbit)
{
  ROS_INFO("I got a new target orbit!");
  ROS_INFO("Activating the thrusters to achieve new orbit");

  satellite_flight_application::SatelliteThrusterControl srv;
  if (thrusterControlClient.call(srv))
    {
      ROS_INFO("Successfully activated satellite thrusters");
    }
  else
    {
      ROS_ERROR("Failed to activate satellite thrusters");
      thrusterControlClient.waitForExistence(ros::Duration(-1));
    }
}

int main(int argc, char **argv)
{
  if ( argc != 2)
    {
      ROS_INFO("usage: BusActor <satellite name>");
      return 1;
    }
  std::string nodeName = "OrbitControllerActor";
  nodeName += argv[1];

  ros::init(argc, argv, nodeName.c_str());

  ros::NodeHandle n;

  std::string stateServiceName = "SatelliteStateVector";
  stateServiceName += argv[1];

  ros::ServiceClient satStateClient = n.serviceClient<satellite_flight_application::SatelliteStateVector>(stateServiceName.c_str());

  std::string thrusterServiceName = "SatelliteThrusterControl";
  thrusterServiceName += argv[1];

  thrusterControlClient = n.serviceClient<satellite_flight_application::SatelliteThrusterControl>(thrusterServiceName.c_str());

  ros::Publisher satStatePub = n.advertise<satellite_flight_application::SatelliteState>("SatelliteState", 1000);

  ros::Subscriber targetOrbitSub = n.subscribe("TargetOrbit", 1000, targetOrbitCallback);

  satStateClient.waitForExistence(ros::Duration(-1));
  thrusterControlClient.waitForExistence(ros::Duration(-1));
  
  ros::Rate loop_rate(1);

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
	  satStateClient.waitForExistence(ros::Duration(-1));
	}

      satellite_flight_application::SatelliteState satState;

      satStatePub.publish(satState);

      ros::spinOnce();

      loop_rate.sleep();
    }

  return 0;
}
