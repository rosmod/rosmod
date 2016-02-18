#include <cstdlib>
#include <string.h>
#include <dlfcn.h>
#include <exception>      
#include <cstdlib>        
#include <signal.h>
#include <boost/thread.hpp>
#include "node/Component.hpp"
#include "pthread.h"
#include "sched.h"

#ifdef USE_ROSMOD
  #include "rosmod/rosmod_ros.h"
#else
  #ifdef USE_ROSCPP
    #include "ros/ros.h"
  #endif
#endif

void componentThreadFunc(Component* compPtr)
{
  compPtr->startUp();
  compPtr->process_queue();
}

std::vector<boost::thread*> compThreads;	

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

  XMLParser nodeParser;
  std::string configFileName = nodeName + ".xml";
  if (configFile.length() > 0)
    configFileName = configFile;
  if (nodeParser.Parse(configFileName))
  {      
    int ret;
    pthread_t this_thread = pthread_self(); 

    // struct sched_param is used to store the scheduling priority
    struct sched_param params;     
    // We'll set the priority to the maximum.     
    params.sched_priority = nodeParser.node_priority;  //sched_get_priority_max(SCHED_RR);
    if (params.sched_priority < 0)
      params.sched_priority = sched_get_priority_max(SCHED_RR);

    std::cout << "Trying to set thread realtime prio = " << 
      params.sched_priority << std::endl; 
     
    // Attempt to set thread real-time priority to the SCHED_FIFO policy     
    ret = pthread_setschedparam(this_thread, SCHED_RR, &params);     
    if (ret != 0)
      std::cout << "Unsuccessful in setting thread realtime prio" << std::endl;

    // Now verify the change in thread priority     
    int policy = 0;     
    ret = pthread_getschedparam(this_thread, &policy, &params);     
    if (ret != 0)          
      std::cout << "Couldn't retrieve real-time scheduling paramers" << std::endl;      
    // Check the correct policy was applied     
    if(policy != SCHED_RR)         
      std::cout << "Scheduling is NOT SCHED_RR!" << std::endl;     
    else
      std::cout << "SCHED_RR OK" << std::endl;     
    // Print thread scheduling priority     
    std::cout << "Thread priority is " << params.sched_priority << std::endl;
  
    for (int i=0;i<nodeParser.libList.size();i++)
    {
      void *hndl = dlopen(nodeParser.libList[i].c_str(), RTLD_LAZY | RTLD_GLOBAL);
      if (hndl == NULL)
      {
	std::cerr << dlerror() << std::endl;
	exit(-1);
      }
      else
	ROS_INFO_STREAM("Opened " << nodeParser.libList[i]);
    }

    nodeName = nodeParser.nodeName;
    NAMESPACE::init(argc, argv, nodeName.c_str());

    // Create Node Handle
    NAMESPACE::NodeHandle n;

    ROS_INFO_STREAM(nodeName << " thread id = " << boost::this_thread::get_id());
    
    for (int i=0;i<nodeParser.compConfigList.size();i++)
    {
      std::string libraryLocation = nodeParser.compConfigList[i].libraryLocation;
      void *hndl = dlopen(libraryLocation.c_str(), RTLD_NOW);
      if(hndl == NULL)
      {
	std::cerr << dlerror() << std::endl;
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

