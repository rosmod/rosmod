#include "ros/ros.h"
#include <cstdlib>
#include <string.h>

// Required for boost::thread
#include <boost/thread.hpp>

// Include all components this actor requires
#include "cluster_flight_application/TrajectoryPlanner_def.hpp" 


void componentThread(Component* compPtr)
{
    compPtr->startUp();
    compPtr->processQueue();
}

int main(int argc, char **argv)
{
    std::string nodeName = "clusterFlightNode";
    std::string hostName = "localhost";

    for(int i = 0; i < argc; i++)
    {
        if(!strcmp(argv[i], "-nodename"))
            nodeName = argv[i+1];
	if(!strcmp(argv[i], "-hostname"))
	    hostName = argv[i+1];
    }

    ros::init(argc, argv, nodeName.c_str());

    // Create Node Handle
    ros::NodeHandle n;

    // Create Component Objects
    TrajectoryPlanner_def trajectoryPlanner_comp(hostName, nodeName, "trajectoryPlanner_comp", argc, argv); 

    // Create Component Threads
    boost::thread trajectoryPlanner_comp_thread(componentThread, &trajectoryPlanner_comp);
    ROS_INFO("Node clusterFlightNode has started trajectoryPlanner_comp");


    ROS_INFO_STREAM("NodeMain thread id = " << boost::this_thread::get_id());

    // Create Component Threads
    trajectoryPlanner_comp_thread.join();


    return 0; 
}

