#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <iostream>
#include <string>
#include "ros/ros.h"
#include <ros/callback_queue.h>
#include "New_Package/xmlParser.hpp"
#include "New_Package/Logger.hpp"

class Component
{
    public:
        // Component Constructor
        Component(std::string hostName, std::string nodeName, std::string compName, int argc, char **argv);

	// StartUp will be completely generated
	virtual void startUp() = 0;

	// Init will be generated with BL supplied by user
	virtual void Init(const ros::TimerEvent& event);

	// queueThread processes queue actions
	void processQueue();

	// required for clean shutdown
	~Component();

    protected:
        std::string hostName;
        std::string nodeName;
        std::string compName;
        int node_argc;
        char **node_argv;
	ros::Timer initOneShotTimer;  // timer for calling init
	ros::CallbackQueue compQueue; // single callbackQueue for the component
        Logger LOGGER;
};

// typedef to make it easier to set up our factory
//typedef Component *maker_t();

#endif
