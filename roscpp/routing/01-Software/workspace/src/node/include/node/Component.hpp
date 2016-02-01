#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <iostream>
#include <string>
#include <std_msgs/Bool.h>
#include "ros/ros.h"
#include "ros/callback_queue.h"
#include "node/xmlParser.hpp"
#include "node/Logger.hpp"

class Component {
public:
  // Constructor
  Component(ComponentConfig &_config, int argc, char **argv);

  // Start up
  virtual void startUp() = 0;

  // Initialization
  virtual void Init(const ros::TimerEvent& event);

  // Synchronization
  virtual void component_synchronization_OnOneData(const std_msgs::Bool::ConstPtr& received_data);

  // Callback Queue Handler
  void processQueue();

  // Destructor
  ~Component();

protected:
  ComponentConfig config;
  std::map<std::string,std::string> portGroupMap;
  Log_Levels logLevels;
  std::string hostName;
  std::string nodeName;
  std::string compName;
  int node_argc;
  char **node_argv;
  ros::Publisher comp_sync_pub;
  ros::Subscriber comp_sync_sub;
  uint64_t num_comps_to_sync;
  double comp_sync_timeout;
  ros::Timer initOneShotTimer; 
  ros::CallbackQueue compQueue;
  Logger LOGGER;
};

#endif
