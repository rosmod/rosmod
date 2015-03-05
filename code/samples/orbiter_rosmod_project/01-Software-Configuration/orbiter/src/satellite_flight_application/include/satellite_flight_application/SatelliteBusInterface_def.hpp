#ifndef SATELLITEBUSINTERFACE_DEF_HPP
#define SATELLITEBUSINTERFACE_DEF_HPP

#include "ros/ros.h"
#include "Component.hpp"

// --------------------------------
//      USER INCLUDES GO HERE
// --------------------------------
//# Start User Includes Marker

#include <sys/types.h>
#include <sys/socket.h>
//#include <netdb.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <ifaddrs.h>

#include <cstring>      // std::memcpy
#include <algorithm>    // std::swap
#include <memory>
//# End User Includes Marker

#include "satellite_flight_application/SatelliteState.h"
#include "satellite_flight_application/ThrusterComm.h"

//# Start User Globals Marker

//# End User Globals Marker

class SatelliteBusInterface_def : public Component
{
    public:
        // Component SatelliteBusInterface_def Constructor
        SatelliteBusInterface_def(std::string nodeName, std::string compName, int argc, char **argv) : Component(nodeName, compName, argc, argv) {}

        // These functions' business logic will be filled in by the user:

	// Init() is always generated
	void Init(const ros::TimerEvent& event);

	// Component Service Callback
	bool SatelliteStateCallback(satellite_flight_application::SatelliteState::Request &req,
		satellite_flight_application::SatelliteState::Response &res);

	// Component Service Callback
	bool ThrusterCommCallback(satellite_flight_application::ThrusterComm::Request &req,
		satellite_flight_application::ThrusterComm::Response &res);


	// these functions' business logic will be auto-generated:

	// startUp() is used to configure timers, publishers, & service providers
	void startUp();

	// required for clean shutdown
	~SatelliteBusInterface_def();

    private:

	// ROS Service Server - SatelliteState_server
	ros::ServiceServer SatelliteState_server;

	// ROS Service Server - ThrusterComm_server
	ros::ServiceServer ThrusterComm_server;


        //# Start User Private Variables Marker

        //# End User Private Variables Marker
};


#endif
