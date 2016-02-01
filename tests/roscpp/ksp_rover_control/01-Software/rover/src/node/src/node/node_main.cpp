#include "ros/ros.h"
#include <cstdlib>
#include <string.h>
#include <dlfcn.h>
#include <exception>      
#include <cstdlib>        
#include <signal.h>
#include <boost/thread.hpp>
#include "node/Component.hpp"

void componentThreadFunc(Component* compPtr)
{
  compPtr->startUp();
  compPtr->processQueue();
}

std::vector<Component*> compObjects;
std::vector<boost::thread*> compThreads;	

void signal_handler(int sig) {
  for (int i=0;i<compThreads.size();i++)
    compThreads[i]->join();
  for (auto it = compObjects.begin(); it != compObjects.end(); ++it)
    delete *it;
  abort(); 
}

int main(int argc, char **argv)
{
  std::string nodeName = "node";
  std::string hostName = "localhost";
  std::string configFile = "";

  signal (SIGINT, &signal_handler);
  signal (SIGTERM, &signal_handler);
  signal (SIGKILL, &signal_handler);
  
  for(int i = 0; i < argc; i++)
  {
    if(!strcmp(argv[i], "-nodename"))
      nodeName = argv[i+1];
    if(!strcmp(argv[i], "-hostname"))
      hostName = argv[i+1];
    if(!strcmp(argv[i], "-config"))
      configFile = argv[i+1];
  }

  XMLParser nodeParser;
  std::string configFileName = nodeName + ".xml";
  if (configFile.length() > 0)
    configFileName = configFile;
  if (nodeParser.Parse(configFileName))
  {      

    for (int i=0;i<nodeParser.libList.size();i++)
    {
      void *hndl = dlopen(nodeParser.libList[i].c_str(), RTLD_LAZY | RTLD_GLOBAL);
      if (hndl == NULL)
      {
	cerr << dlerror() << endl;
	exit(-1);
      }
      else
	ROS_INFO_STREAM("Opened " << nodeParser.libList[i]);
    }

    nodeName = nodeParser.nodeName;
    ros::init(argc, argv, nodeName.c_str());

    // Create Node Handle
    ros::NodeHandle n;

    ROS_INFO_STREAM(nodeName << " thread id = " << boost::this_thread::get_id());
    
    for (int i=0;i<nodeParser.compConfigList.size();i++)
    {
      std::string libraryLocation = nodeParser.compConfigList[i].libraryLocation;
      void *hndl = dlopen(libraryLocation.c_str(), RTLD_NOW);
      if(hndl == NULL)
      {
	cerr << dlerror() << endl;
	exit(-1);
      }
      void *mkr = dlsym(hndl, "maker");
      Component *comp_inst = ((Component *(*)(ComponentConfig &, int , char **))(mkr))
	(nodeParser.compConfigList[i], argc, argv);

      // Push to object vector
      compObjects.push_back(comp_inst);

      // Create Component Threads
      boost::thread *comp_thread = new boost::thread(componentThreadFunc, comp_inst);
      compThreads.push_back(comp_thread);
      ROS_INFO_STREAM(nodeName << " has started " << nodeParser.compConfigList[i].compName);
    }
    for (int i=0;i<compThreads.size();i++)
    {
      compThreads[i]->join();
    }
    return 0; 
  }
  else
  {
    printf("ERROR::Unable to parse XML file\n");
    return -1;
  }
}

