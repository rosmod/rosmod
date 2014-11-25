#include "ros/ros.h"
#include "satellite_flight_application/SatelliteStateVector.h"
#include "satellite_flight_application/SatelliteThrusterControl.h"

bool getSatStateVector(satellite_flight_application::SatelliteStateVector::Request  &req,
		       satellite_flight_application::SatelliteStateVector::Response &res)
{
  ROS_INFO("Returning the current satellite state vector.");
  return true;
}

bool activateSatelliteThruster(satellite_flight_application::SatelliteThrusterControl::Request  &req,
			       satellite_flight_application::SatelliteThrusterControl::Response &res)
{
  ROS_INFO("Activating the satellite thruster.");
  return true;
}

int main(int argc, char **argv)
{
  if ( argc != 2)
    {
      ROS_INFO("usage: BusActor <satellite name>");
      return 1;
    }
  std::string nodeName = "BusActor";
  nodeName += argv[1];

  ros::init(argc, argv, nodeName.c_str());
  ros::NodeHandle n;

  std::string stateVectorServiceName = "SatelliteStateVector";
  stateVectorServiceName += argv[1];

  std::string thrusterServiceName = "SatelliteThrusterControl";
  thrusterServiceName += argv[1];

  ros::ServiceServer satStateVectorService = n.advertiseService(stateVectorServiceName, getSatStateVector);
  ros::ServiceServer satThrusterControlService = n.advertiseService(thrusterServiceName, activateSatelliteThruster);

  ros::spin();

  return 0;
}
