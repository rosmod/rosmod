#ifndef HIGHRESOLUTIONIMAGEPRODUCER_DEF_HPP
#define HIGHRESOLUTIONIMAGEPRODUCER_DEF_HPP

#include "ros/ros.h"
#include "Component.hpp"

// --------------------------------
//      USER INCLUDES GO HERE
// --------------------------------
//# Start User Includes Marker

//# End User Includes Marker

#include "wam_application/HRImageVector.h"

//# Start User Globals Marker

//# End User Globals Marker

class HighResolutionImageProducer_def : public Component
{
    public:
        // Component HighResolutionImageProducer_def Constructor
        HighResolutionImageProducer_def(std::string nodeName, std::string compName, int argc, char **argv) : Component(nodeName, compName, argc, argv) {}

        // These functions' business logic will be filled in by the user:

	// Init() is always generated
	void Init(const ros::TimerEvent& event);

	// Callback for Timer0 timer
	void Timer0Callback(const ros::TimerEvent& event);


	// these functions' business logic will be auto-generated:

	// startUp() is used to configure timers, publishers, & service providers
	void startUp();

	// required for clean shutdown
	~HighResolutionImageProducer_def();

    private:

	// ROS Timer - Timer0
	ros::Timer Timer0;


	// ROS Publisher - hrImage_pub
	ros::Publisher hrImage_pub;


        //# Start User Private Variables Marker

        //# End User Private Variables Marker
};


#endif
