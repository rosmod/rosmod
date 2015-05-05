#include "ros/ros.h"
#include <cstdlib>
#include <string.h>

// Required for boost::thread
#include <boost/thread.hpp>

// Include all components this actor requires
#include "New_Package/dlcomp.hpp" 
#include "New_Package/dlcomp.hpp" 


void componentThread(Component* compPtr)
{
    compPtr->startUp();
    compPtr->processQueue();
}

int main(int argc, char **argv)
{
    std::string nodeName = "node1";
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
    dlcomp dlcomp_1(hostName, nodeName, "dlcomp_1", argc, argv); 
    dlcomp dlcomp_2(hostName, nodeName, "dlcomp_2", argc, argv); 

    // Create Component Threads
    boost::thread dlcomp_1_thread(componentThread, &dlcomp_1);
    ROS_INFO("Node node1 has started dlcomp_1");
    boost::thread dlcomp_2_thread(componentThread, &dlcomp_2);
    ROS_INFO("Node node1 has started dlcomp_2");


    ROS_INFO_STREAM("NodeMain thread id = " << boost::this_thread::get_id());

    // Create Component Threads
    dlcomp_1_thread.join();
    dlcomp_2_thread.join();


    return 0; 
}

