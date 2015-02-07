#ifndef COMPONENT_2_HPP
#define COMPONENT_2_HPP

#include "ros/ros.h"
#include "Component.hpp"

#include "Package_1/ComponentName.h"
#include "Package_1/ComponentService.h"

// --------------------------------
//      USER INCLUDES GO HERE
// --------------------------------

class Component_2 : public Component
{
    public:

        // These functions' business logic will be filled in by the user:

	// Init() is always generated
	void Init(const ros::TimerEvent& event);

	// Component Service Callback
	bool ComponentServiceCallback(Package_1::ComponentService::Request  &req,
		Package_1::ComponentService::Response &res);


	// Callback for Timer0 timer
	void Timer0Callback(const ros::TimerEvent& event);


	// these functions' business logic will be auto-generated:

	// startUp() is used to configure timers, publishers, & service providers
	void startUp();

	// required for clean shutdown
	~Component_2();

    private:

	// ROS Timer - Timer0
	ros::Timer Timer0;


	// ROS Publisher - Component_2_publisher
	ros::Publisher Component_2_publisher;


	// ROS Service Server - ComponentService_server
	ros::ServiceServer ComponentService_server;


};


#endif
