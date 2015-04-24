#ifndef KRPC_COMP_HPP
#define KRPC_COMP_HPP

#include "ros/ros.h"
#include "Component.hpp"

// --------------------------------
//      USER INCLUDES GO HERE
// --------------------------------
//# Start User Includes Marker
#include "krpc/KRPC.pb.h"
#include <boost/bind.hpp>
#include <boost/asio.hpp>
//# End User Includes Marker


//# Start User Globals Marker
using boost::asio::ip::tcp;
//# End User Globals Marker

class krpc_comp : public Component
{
    public:
        // Component krpc_comp Constructor
        krpc_comp(std::string hostName, std::string nodeName, std::string compName, int argc, char **argv) : Component(hostName, nodeName, compName, argc, argv) {}

        // These functions' business logic will be filled in by the user:

	// Init() is always generated
	void Init(const ros::TimerEvent& event);

	// Callback for timer0 timer
	void timer0Callback(const ros::TimerEvent& event);


	// these functions' business logic will be auto-generated:

	// startUp() is used to configure timers, publishers, & service providers
	void startUp();

	// required for clean shutdown
	~krpc_comp();

    private:

	// ROS Timer - timer0
	ros::Timer timer0;


        //# Start User Private Variables Marker
        //# End User Private Variables Marker
};


#endif
