#include "ros/ros.h"
#include <cstdlib>
#include <string.h>

// Required for boost::thread
#include <boost/thread.hpp>

// Include all components this actor requires
#include "wam_application/ImageProcessor_def.hpp" 


void componentThread(Component* compPtr)
{
    compPtr->startUp();
    compPtr->processQueue();
}

int main(int argc, char **argv)
{
    std::string nodeName = "imageProcessorNode";

    for(int i = 0; i < argc; i++)
        if(!strcmp(argv[i], "-nodename"))
            nodeName = argv[i+1];

    ros::init(argc, argv, nodeName.c_str());

    // Create Node Handle
    ros::NodeHandle n;

    // Create Component Objects
    ImageProcessor_def image_processor_comp(nodeName, "image_processor_comp", argc, argv); 

    // Create Component Threads
    boost::thread image_processor_comp_thread(componentThread, &image_processor_comp);
    ROS_INFO("Node imageProcessorNode has started image_processor_comp");


    ROS_INFO_STREAM("NodeMain thread id = " << boost::this_thread::get_id());

    // Create Component Threads
    image_processor_comp_thread.join();


    return 0; 
}

