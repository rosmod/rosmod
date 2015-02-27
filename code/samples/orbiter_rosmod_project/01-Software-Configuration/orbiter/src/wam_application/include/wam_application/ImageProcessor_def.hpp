#ifndef IMAGEPROCESSOR_DEF_HPP
#define IMAGEPROCESSOR_DEF_HPP

#include "ros/ros.h"
#include "Component.hpp"

// --------------------------------
//      USER INCLUDES GO HERE
// --------------------------------
//# Start User Includes Marker

//# End User Includes Marker

#include "wam_application/HRImageVector.h"
#include "wam_application/LRImageVector.h"

//# Start User Globals Marker

//# End User Globals Marker

class ImageProcessor_def : public Component
{
    public:
        // Component ImageProcessor_def Constructor
        ImageProcessor_def(std::string nodeName, int argc, char **argv) : Component(nodeName, argc, argv) {}

        // These functions' business logic will be filled in by the user:

	// Init() is always generated
	void Init(const ros::TimerEvent& event);

	// OnOneData Subscription handler for HRsub subscriber 
	void HRsub_OnOneData(const wam_application::HRImageVector::ConstPtr& received_data); 
 
	// OnOneData Subscription handler for LRsub subscriber 
	void LRsub_OnOneData(const wam_application::LRImageVector::ConstPtr& received_data); 
 

	// these functions' business logic will be auto-generated:

	// startUp() is used to configure timers, publishers, & service providers
	void startUp();

	// required for clean shutdown
	~ImageProcessor_def();

    private:

	// ROS Subscriber - HRsub
	ros::Subscriber HRsub; 

	// ROS Subscriber - LRsub
	ros::Subscriber LRsub; 


        //# Start User Private Variables Marker

        //# End User Private Variables Marker
};


#endif
