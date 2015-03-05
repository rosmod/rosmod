#include "ros/ros.h"
#include <cstdlib>
#include <string.h>

// Required for boost::thread
#include <boost/thread.hpp>

// Include all components this actor requires
#include "wam_application/LowResolutionIMageProducer_def.hpp" 


void componentThread(Component* compPtr)
{
    compPtr->startUp();
    compPtr->processQueue();
}

int main(int argc, char **argv)
{
    std::string nodeName = "lowResolutionImager";

    for(int i = 0; i < argc; i++)
        if(!strcmp(argv[i], "-nodename"))
            nodeName = argv[i+1];

    ros::init(argc, argv, nodeName.c_str());

    // Create Node Handle
    ros::NodeHandle n;

    // Create Component Objects
    LowResolutionIMageProducer_def LR_sensor(nodeName, "LR_sensor", argc, argv); 

    // Create Component Threads
    boost::thread LR_sensor_thread(componentThread, &LR_sensor);
    ROS_INFO("Node lowResolutionImager has started LR_sensor");


    ROS_INFO_STREAM("NodeMain thread id = " << boost::this_thread::get_id());

    // Create Component Threads
    LR_sensor_thread.join();


    return 0; 
}

