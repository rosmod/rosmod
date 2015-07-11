#ifndef SATELLITEBUSINTERFACE_HPP
#define SATELLITEBUSINTERFACE_HPP

#include "ros/ros.h"
#include "Component.hpp"
#include "satellite_flight_application/SatelliteState.h"
#include "satellite_flight_application/ThrusterComm.h"

#include "krpci/krpci.hpp"

//# Start User Includes Marker
#include "krpci/krpci.hpp"
//# End User Includes Marker

//# Start User Globals Marker
//# End User Globals Marker

class SatelliteBusInterface : public Component
{
public:
  // Constructor
  SatelliteBusInterface(ComponentConfig& config, int argc, char **argv) : Component(config, argc, argv) {}

  // Initialization
  void Init(const ros::TimerEvent& event);

  // Server Callback - SatelliteState_Server
  bool SatelliteStateCallback(satellite_flight_application::SatelliteState::Request &req, 
    satellite_flight_application::SatelliteState::Response &res);

  // Server Callback - ThrusterComm_Server
  bool ThrusterCommCallback(satellite_flight_application::ThrusterComm::Request &req, 
    satellite_flight_application::ThrusterComm::Response &res);

  // Start up
  void startUp();

  // Destructor
  ~SatelliteBusInterface();

private:

  // Server 
  ros::ServiceServer SatelliteState_Server;

  // Server 
  ros::ServiceServer ThrusterComm_Server;

  //# Start User Private Variables Marker
  KRPCI krpc_client;
  uint64_t vessel_id;
  std::string vessel_name;
  uint64_t control_id;
  uint64_t orbit_id;
  uint64_t body_id;
  uint64_t reference_frame_id;
  //# End User Private Variables Marker
};

#endif
