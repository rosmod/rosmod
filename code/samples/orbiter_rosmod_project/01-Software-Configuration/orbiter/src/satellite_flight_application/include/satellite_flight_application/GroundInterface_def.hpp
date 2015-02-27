#ifndef GROUNDINTERFACE_DEF_HPP
#define GROUNDINTERFACE_DEF_HPP

#include "ros/ros.h"
#include "Component.hpp"

// --------------------------------
//      USER INCLUDES GO HERE
// --------------------------------
//# Start User Includes Marker

//# End User Includes Marker

#include "satellite_flight_application/GroundCommand.h"

//# Start User Globals Marker

//# End User Globals Marker

class GroundInterface_def : public Component
{
    public:
        // Component GroundInterface_def Constructor
        GroundInterface_def(std::string nodeName, int argc, char **argv) : Component(nodeName, argc, argv) {}

        // These functions' business logic will be filled in by the user:

	// Init() is always generated
	void Init(const ros::TimerEvent& event);

	// Callback for Timer0 timer
	void Timer0Callback(const ros::TimerEvent& event);


	// these functions' business logic will be auto-generated:

	// startUp() is used to configure timers, publishers, & service providers
	void startUp();

	// required for clean shutdown
	~GroundInterface_def();

    private:

	// ROS Timer - Timer0
	ros::Timer Timer0;


	// ROS Publisher - gndCommandPub
	ros::Publisher gndCommandPub;


        //# Start User Private Variables Marker
  int sockfd;
        //# End User Private Variables Marker
};


#endif
