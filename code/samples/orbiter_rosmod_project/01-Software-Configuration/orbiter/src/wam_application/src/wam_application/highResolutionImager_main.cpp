#include "ros/ros.h"
#include <cstdlib>
#include <string.h>

// Required for boost::thread
#include <boost/thread.hpp>

// Include all components this actor requires
#include "wam_application/HighResolutionImageProducer_def.hpp" 


void componentThread(Component* compPtr)
{
    compPtr->startUp();
    compPtr->processQueue();
}

int main(int argc, char **argv)
{
    std::string nodeName = "highResolutionImager";

    for(int i = 0; i < argc; i++)
        if(!strcmp(argv[i], "-nodename"))
            nodeName = argv[i+1];

    ros::init(argc, argv, nodeName.c_str());

    // Create Node Handle
    ros::NodeHandle n;

    // Create Component Objects
    HighResolutionImageProducer_def HR_sensor(nodeName, argc, argv); 

    // Create Component Threads
    boost::thread HR_sensor_thread(componentThread, &HR_sensor);
    ROS_INFO("Node highResolutionImager has started HR_sensor");


    ROS_INFO_STREAM("NodeMain thread id = " << boost::this_thread::get_id());

    // Create Component Threads
    HR_sensor_thread.join();


    return 0; 
}

