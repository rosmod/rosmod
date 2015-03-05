#ifndef ORBITCONTROLLER_DEF_HPP
#define ORBITCONTROLLER_DEF_HPP

#include "ros/ros.h"
#include "Component.hpp"

// --------------------------------
//      USER INCLUDES GO HERE
// --------------------------------
//# Start User Includes Marker

//# End User Includes Marker

#include "satellite_flight_application/SatState.h"
#include "cluster_flight_application/TargetOrbit.h"
#include "satellite_flight_application/SatelliteState.h"
#include "satellite_flight_application/ThrusterComm.h"

//# Start User Globals Marker

//# End User Globals Marker

class OrbitController_def : public Component
{
    public:
        // Component OrbitController_def Constructor
        OrbitController_def(std::string nodeName, std::string compName, int argc, char **argv) : Component(nodeName, compName, argc, argv) {}

        // These functions' business logic will be filled in by the user:

	// Init() is always generated
	void Init(const ros::TimerEvent& event);

	// OnOneData Subscription handler for targetOrbitSub subscriber 
	void targetOrbitSub_OnOneData(const cluster_flight_application::TargetOrbit::ConstPtr& received_data); 
 

	// Callback for Timer0 timer
	void Timer0Callback(const ros::TimerEvent& event);


	// these functions' business logic will be auto-generated:

	// startUp() is used to configure timers, publishers, & service providers
	void startUp();

	// required for clean shutdown
	~OrbitController_def();

    private:

	// ROS Timer - Timer0
	ros::Timer Timer0;


	// ROS Subscriber - targetOrbitSub
	ros::Subscriber targetOrbitSub; 


	// ROS Publisher - satStatePub
	ros::Publisher satStatePub;


	// ROS Service Client - SatelliteState_client
	ros::ServiceClient SatelliteState_client;

	// ROS Service Client - ThrusterComm_client
	ros::ServiceClient ThrusterComm_client;

        //# Start User Private Variables Marker

        //# End User Private Variables Marker
};


#endif
