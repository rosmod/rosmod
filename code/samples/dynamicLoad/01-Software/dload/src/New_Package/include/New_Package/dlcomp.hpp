#ifndef DLCOMP_HPP
#define DLCOMP_HPP

#include "ros/ros.h"
#include "Component.hpp"

// --------------------------------
//      USER INCLUDES GO HERE
// --------------------------------
//# Start User Includes Marker

//# End User Includes Marker

#include "New_Package/dlmsg.h"
#include "New_Package/dlmsg.h"
#include "New_Package/dlsrv.h"
#include "New_Package/dlsrv.h"

//# Start User Globals Marker

//# End User Globals Marker

class dlcomp : public Component
{
    public:
        // Component dlcomp Constructor
        dlcomp(std::string hostName, std::string nodeName, std::string compName, int argc, char **argv) : Component(hostName, nodeName, compName, argc, argv) {}

        // These functions' business logic will be filled in by the user:

	// Init() is always generated
	void Init(const ros::TimerEvent& event);

	// OnOneData Subscription handler for sub subscriber 
	void sub_OnOneData(const New_Package::dlmsg::ConstPtr& received_data); 
 

	// Component Service Callback
	bool dlsrvCallback(New_Package::dlsrv::Request &req,
		New_Package::dlsrv::Response &res);


	// Callback for timer0 timer
	void timer0Callback(const ros::TimerEvent& event);


	// these functions' business logic will be auto-generated:

	// startUp() is used to configure timers, publishers, & service providers
	void startUp();

	// required for clean shutdown
	~dlcomp();

    private:

	// ROS Timer - timer0
	ros::Timer timer0;


	// ROS Subscriber - sub
	ros::Subscriber sub; 


	// ROS Publisher - pub
	ros::Publisher pub;


	// ROS Service Server - dlsrv_server
	ros::ServiceServer dlsrv_server;


	// ROS Service Client - dlsrv_client
	ros::ServiceClient dlsrv_client;

        //# Start User Private Variables Marker

        //# End User Private Variables Marker
};


#endif
