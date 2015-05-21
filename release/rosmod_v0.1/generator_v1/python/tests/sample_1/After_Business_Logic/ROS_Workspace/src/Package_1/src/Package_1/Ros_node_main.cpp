#include "ros/ros.h"
#include <cstdlib>

// Required for boost::thread
#include <boost/thread.hpp>

// Include all components this actor requires
#include "Package_1/Component_1.hpp" 
#include "Package_1/Component_2.hpp" 
#include "Package_1/Component_3.hpp" 


void componentThread(Component* compPtr)
{
    compPtr->startUp();
    compPtr->processQueue();
}

int main(int argc, char **argv)
{
    std::string nodeName = "Ros_node";
    ros::init(argc, argv, nodeName.c_str());

    // Create Node Handle
    ros::NodeHandle n;

    // Create Component Objects
    Component_1 Component_1_i; 
    Component_2 Component_2_i; 
    Component_3 Component_3_i; 

    // Create Component Threads
    boost::thread Component_1_i_thread(componentThread, &Component_1_i);
    ROS_INFO("Node Ros_node has started Component_1_i");
    boost::thread Component_2_i_thread(componentThread, &Component_2_i);
    ROS_INFO("Node Ros_node has started Component_2_i");
    boost::thread Component_3_i_thread(componentThread, &Component_3_i);
    ROS_INFO("Node Ros_node has started Component_3_i");


    ROS_INFO_STREAM("NodeMain thread id = " << boost::this_thread::get_id());

    // Create Component Threads
    Component_1_i_thread.join();
    Component_2_i_thread.join();
    Component_3_i_thread.join();


    return 0; 
}

