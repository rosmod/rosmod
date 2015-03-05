#include "ros/ros.h"
#include <cstdlib>
#include <string.h>

// Required for boost::thread
#include <boost/thread.hpp>

// Include all components this actor requires
#include "satellite_flight_application/GroundInterface_def.hpp" 
#include "satellite_flight_application/OrbitController_def.hpp" 
#include "satellite_flight_application/SatelliteBusInterface_def.hpp" 


void componentThread(Component* compPtr)
{
    compPtr->startUp();
    compPtr->processQueue();
}

int main(int argc, char **argv)
{
    std::string nodeName = "satelliteFlightNode";

    for(int i = 0; i < argc; i++)
        if(!strcmp(argv[i], "-nodename"))
            nodeName = argv[i+1];

    ros::init(argc, argv, nodeName.c_str());

    // Create Node Handle
    ros::NodeHandle n;

    // Create Component Objects
    GroundInterface_def groundInterface_comp(nodeName, "groundInterface_comp", argc, argv); 
    OrbitController_def orbitController_comp(nodeName, "orbitController_comp", argc, argv); 
    SatelliteBusInterface_def satBusIntf_comp(nodeName, "satBusIntf_comp", argc, argv); 

    // Create Component Threads
    boost::thread groundInterface_comp_thread(componentThread, &groundInterface_comp);
    ROS_INFO("Node satelliteFlightNode has started groundInterface_comp");
    boost::thread orbitController_comp_thread(componentThread, &orbitController_comp);
    ROS_INFO("Node satelliteFlightNode has started orbitController_comp");
    boost::thread satBusIntf_comp_thread(componentThread, &satBusIntf_comp);
    ROS_INFO("Node satelliteFlightNode has started satBusIntf_comp");


    ROS_INFO_STREAM("NodeMain thread id = " << boost::this_thread::get_id());

    // Create Component Threads
    groundInterface_comp_thread.join();
    orbitController_comp_thread.join();
    satBusIntf_comp_thread.join();


    return 0; 
}

