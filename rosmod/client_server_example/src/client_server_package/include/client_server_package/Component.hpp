#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <iostream>
#include <string>
#include "rosmod/rosmod_ros.h"
#include "rosmod/rosmod_callback_queue.h"
#include "client_server_package/xmlParser.hpp"
#include "client_server_package/Logger.hpp"

class Component {
public:
  // Constructor
  Component(ComponentConfig &config, int argc, char **argv);

  // Start up
  virtual void startUp() = 0;

  // Initialization
  virtual void Init(const rosmod::TimerEvent& event);

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
  rosmod::Timer initOneShotTimer; 
  rosmod::CallbackQueue compQueue;
  Logger LOGGER;
};

#endif
