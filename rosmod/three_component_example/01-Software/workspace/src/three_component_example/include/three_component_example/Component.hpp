#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <iostream>
#include <string>
#include <std_msgs/Bool.h>
#include "rosmod/rosmod_ros.h"
#include "rosmod/rosmod_callback_queue.h"
#include "three_component_example/xmlParser.hpp"
#include "three_component_example/Logger.hpp"

class Component {
public:
  // Constructor
  Component(ComponentConfig &config, int argc, char **argv);

  // Start up
  virtual void startUp() = 0;

  // Initialization
  virtual void Init(const rosmod::TimerEvent& event);

  // Synchronization
  virtual void component_synchronization_OnOneData(const std_msgs::Bool::ConstPtr& received_data);

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
  rosmod::CallbackQueue::SchedulingScheme scheduling_scheme;
  int node_argc;
  char **node_argv;
  rosmod::Publisher comp_sync_pub;
  rosmod::Subscriber comp_sync_sub;
  uint64_t num_comps_to_sync;
  double comp_sync_timeout;
  rosmod::Timer initOneShotTimer; 
  rosmod::CallbackQueue compQueue;
  Logger LOGGER;
};

#endif
