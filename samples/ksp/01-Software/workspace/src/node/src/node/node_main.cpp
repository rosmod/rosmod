#include "ros/ros.h"
#include <cstdlib>
#include <string.h>
#include <dlfcn.h>
#include <boost/thread.hpp>
#include "node/Component.hpp"

void componentThreadFunc(Component* compPtr)
{
  compPtr->startUp();
  compPtr->processQueue();
}

int main(int argc, char **argv)
{
  std::string nodeName = "node";
  std::string hostName = "localhost";
  std::string configFile = "";
  
  for(int i = 0; i < argc; i++)
  {
    if(!strcmp(argv[i], "-nodename"))
      nodeName = argv[i+1];
    if(!strcmp(argv[i], "-hostname"))
      hostName = argv[i+1];
    if(!strcmp(argv[i], "-config"))
      configFile = argv[i+1];
  }

  std::vector<boost::thread*> compThreads;
  XMLParser nodeParser;
  std::string configFileName = nodeName + ".xml";
  if (configFile.length() > 0)
    configFileName = configFile;
  if (nodeParser.Parse(configFileName))
  {      
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

