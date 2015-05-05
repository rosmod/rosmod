#include "ros/ros.h"
#include <cstdlib>
#include <string.h>

#include <dlfcn.h>

// Required for boost::thread
#include <boost/thread.hpp>

// Include all components this actor requires
#include "New_Package/Component.hpp" 

void componentThread(Component* compPtr)
{
    compPtr->startUp();
    compPtr->processQueue();
}

int main(int argc, char **argv)
{

  // NEED TO PARSE XML HERE TO GET:
  // * NODE NAME
  // * HOST NAME
  // * COMPONENT SO FILES
  // * COMPONENT PORT PROPERTEIS (HANDLE IN COMPONENT?)
    std::string nodeName = "node1";
    std::string hostName = "localhost";

    for(int i = 0; i < argc; i++)
    {
        if(!strcmp(argv[i], "-nodename"))
            nodeName = argv[i+1];
	if(!strcmp(argv[i], "-hostname"))
	    hostName = argv[i+1];
    }

    ros::init(argc, argv, nodeName.c_str());

    // Create Node Handle
    ros::NodeHandle n;

    // THIS WOULD NEED TO BE IN A LOOP FOR EACH OF THE INSTANCES AND THE COMPONENT DEFS
    void *hndl = dlopen("libdlcomp_lib.so", RTLD_NOW);
    if(hndl == NULL){
      cerr << dlerror() << endl;
      exit(-1);
    }
    void *mkr = dlsym(hndl, "maker");
    Component *comp_inst = ((Component *(*)(std::string , std::string , std::string , int , char **))(mkr))(hostName, nodeName, "comp_inst_name_1", argc, argv);

    // Create Component Threads
    boost::thread dlcomp_1_thread(componentThread, comp_inst);
    ROS_INFO("Node node1 has started dlcomp_1");

    ROS_INFO_STREAM("NodeMain thread id = " << boost::this_thread::get_id());

    // Create Component Threads
    dlcomp_1_thread.join();


    return 0; 
}

