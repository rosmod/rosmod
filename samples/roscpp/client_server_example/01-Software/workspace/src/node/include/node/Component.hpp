#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <iostream>
#include <string>
#include "ros/ros.h"
#include <ros/callback_queue.h>
#include "node/xmlParser.hpp"
#include "node/Logger.hpp"

class Component {
public:
  // Constructor
  Component(ComponentConfig &config, int argc, char **argv);

  // Start up
  virtual void startUp() = 0;

  // Initialization
  virtual void Init(const ros::TimerEvent& event);

  // Callback Queue Handler
  void processQueue();

  // Destructor
  ~Component();

protected:
  std::map<std::string,std::string> portGroupMap;
  Log_Levels logLevels;
  std::string hostName;
  std::string nodeName;
  std::string compName;
  int node_argc;
  char **node_argv;
  ros::Timer initOneShotTimer; 
  ros::CallbackQueue compQueue;
  Logger LOGGER;
};

#endif
