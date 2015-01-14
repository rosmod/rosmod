#ifndef COMPONENT_3_HPP
#define COMPONENT_3_HPP

#include "ros/ros.h"
#include "Component.hpp"

#include "Package_1/ComponentService.h"

// --------------------------------
//      USER INCLUDES GO HERE
// --------------------------------

class Component_3 : public Component
{
    public:

        // These functions' business logic will be filled in by the user:

	// Init() is always generated
	void Init(const ros::TimerEvent& event);

	// Callback for Timer0 timer
	void Timer0Callback(const ros::TimerEvent& event);


	// these functions' business logic will be auto-generated:

	// startUp() is used to configure timers, publishers, & service providers
	void startUp();

	// required for clean shutdown
	~Component_3();

    private:

	// ROS Timer - Timer0
	ros::Timer2 Timer0;


	// ROS Service Client - ComponentService_client
	ros::ServiceClient ComponentService_client;


};


#endif
