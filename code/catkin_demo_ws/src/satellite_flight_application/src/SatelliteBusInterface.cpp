#include "ros/ros.h"
#include "satellite_flight_application/SatelliteState.h"
#include "satellite_flight_application/ThrusterComm.h"

bool getSatStateVector(satellite_flight_application::SatelliteState::Request  &req,
		       satellite_flight_application::SatelliteState::Response &res)
{
  ROS_INFO("Returning the current satellite state vector.");
  return true;
}

bool activateSatelliteThruster(satellite_flight_application::ThrusterComm::Request  &req,
			       satellite_flight_application::ThrusterComm::Response &res)
{
  ROS_INFO("Activating the satellite thruster.");
  return true;
}

int main(int argc, char **argv)
{
  if ( argc != 2)
    {
      ROS_INFO("usage: SatelliteBusInterface_a <satellite name>");
      return 1;
    }
  std::string nodeName = "SatelliteBusInterface_a";
  nodeName += argv[1];

  ros::init(argc, argv, nodeName.c_str());
  ros::NodeHandle n;

  std::string stateVectorServiceName = "SatelliteState";
  stateVectorServiceName += argv[1];

  std::string thrusterServiceName = "ThrusterComm";
  thrusterServiceName += argv[1];

  ros::ServiceServer satStateVectorService = n.advertiseService(stateVectorServiceName, getSatStateVector);
  ros::ServiceServer satThrusterControlService = n.advertiseService(thrusterServiceName, activateSatelliteThruster);

  ROS_INFO("SatelliteBusInterface %s operational.",argv[1]);

  ros::spin();

  return 0;
}
