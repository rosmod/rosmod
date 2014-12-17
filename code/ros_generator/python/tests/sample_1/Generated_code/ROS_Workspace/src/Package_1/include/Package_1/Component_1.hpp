#ifndef COMPONENT_1_HPP
#define COMPONENT_1_HPP

#include "ros/ros.h"
#include "Component.hpp"

#include "Package_1/ComponentName.h"

// --------------------------------
//      USER INCLUDES GO HERE
// --------------------------------

class Component_1 : public Component
{
    public:

        // These functions' business logic will be filled in by the user:

	// Init() is always generated
	void Init(const ros::TimerEvent& event);

	// OnOneData Subscription handler for Component_1_subscriber subscriber 
	void OnOneData(const Package_1::ComponentName::ConstPtr& received_data); 
 

	// Callback for Timer0 timer
	void Timer0Callback(const ros::TimerEvent& event);


	// these functions' business logic will be auto-generated:

	// startUp() is used to configure timers, publishers, & service providers
	void startUp();

	// required for clean shutdown
	~Component_1();

    private:

	// ROS Timer - Timer0
	ros::Timer Timer0;


	// ROS Subscriber - Component_1_subscriber
	ros::Subscriber Component_1_subscriber; 


	// ROS Publisher - Component_1_publisher
	ros::Publisher Component_1_publisher;


};


#endif
