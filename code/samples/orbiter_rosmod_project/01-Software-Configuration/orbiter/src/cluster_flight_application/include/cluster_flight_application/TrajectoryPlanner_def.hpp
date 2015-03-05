#ifndef TRAJECTORYPLANNER_DEF_HPP
#define TRAJECTORYPLANNER_DEF_HPP

#include "ros/ros.h"
#include "Component.hpp"

// --------------------------------
//      USER INCLUDES GO HERE
// --------------------------------
//# Start User Includes Marker

//# End User Includes Marker

#include "cluster_flight_application/TargetOrbit.h"
#include "satellite_flight_application/SatState.h"
#include "satellite_flight_application/GroundCommand.h"

//# Start User Globals Marker

//# End User Globals Marker

class TrajectoryPlanner_def : public Component
{
    public:
        // Component TrajectoryPlanner_def Constructor
        TrajectoryPlanner_def(std::string nodeName, std::string compName, int argc, char **argv) : Component(nodeName, compName, argc, argv) {}

        // These functions' business logic will be filled in by the user:

	// Init() is always generated
	void Init(const ros::TimerEvent& event);

	// OnOneData Subscription handler for satStateSub subscriber 
	void satStateSub_OnOneData(const satellite_flight_application::SatState::ConstPtr& received_data); 
 
	// OnOneData Subscription handler for satCommandSub subscriber 
	void satCommandSub_OnOneData(const satellite_flight_application::GroundCommand::ConstPtr& received_data); 
 

	// these functions' business logic will be auto-generated:

	// startUp() is used to configure timers, publishers, & service providers
	void startUp();

	// required for clean shutdown
	~TrajectoryPlanner_def();

    private:

	// ROS Subscriber - satStateSub
	ros::Subscriber satStateSub; 

	// ROS Subscriber - satCommandSub
	ros::Subscriber satCommandSub; 


	// ROS Publisher - targetOrbitPub
	ros::Publisher targetOrbitPub;


        //# Start User Private Variables Marker

        //# End User Private Variables Marker
};


#endif
