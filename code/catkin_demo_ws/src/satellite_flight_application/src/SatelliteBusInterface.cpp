#include "ros/ros.h"
#include "satellite_flight_application/SatelliteStateVector.h"
#include "satellite_flight_application/SatelliteThrusterControl.h"

bool getSatStateVector(satellite_flight_application::SatelliteStateVector::Request  &req,
		       satellite_flight_application::SatelliteStateVector::Response &res)
{
  return true;
}

bool activateSatelliteThruster(satellite_flight_application::SatelliteThrusterControl::Request  &req,
			       satellite_flight_application::SatelliteThrusterControl::Response &res)
{
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "BusActor");
  ros::NodeHandle n;

  ros::ServiceServer satStateVectorService = n.advertiseService("SatelliteStateVector", getSatStateVector);
  ros::ServiceServer satThrusterControlService = n.advertiseService("SatelliteThrusterControl", activateSatelliteThruster);

  ros::spin();

  return 0;
}
