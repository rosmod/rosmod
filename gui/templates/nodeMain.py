#!/usr/bin/env python




##################################################
## DEPENDENCIES
import sys
import os
import os.path
try:
    import builtins as builtin
except ImportError:
    import __builtin__ as builtin
from os.path import getmtime, exists
import time
import types
from Cheetah.Version import MinCompatibleVersion as RequiredCheetahVersion
from Cheetah.Version import MinCompatibleVersionTuple as RequiredCheetahVersionTuple
from Cheetah.Template import Template
from Cheetah.DummyTransaction import *
from Cheetah.NameMapper import NotFound, valueForName, valueFromSearchList, valueFromFrameOrSearchList
from Cheetah.CacheRegion import CacheRegion
import Cheetah.Filters as Filters
import Cheetah.ErrorCatchers as ErrorCatchers

##################################################
## MODULE CONSTANTS
VFFSL=valueFromFrameOrSearchList
VFSL=valueFromSearchList
VFN=valueForName
currentTime=time.time
__CHEETAH_version__ = '2.4.4'
__CHEETAH_versionTuple__ = (2, 4, 4, 'development', 0)
__CHEETAH_genTime__ = 1455646683.7607
__CHEETAH_genTimestamp__ = 'Tue Feb 16 12:18:03 2016'
__CHEETAH_src__ = '/home/jeb/Repositories/rosmod/gui/templates/nodeMain.tmpl'
__CHEETAH_srcLastModified__ = 'Mon Feb 15 15:36:30 2016'
__CHEETAH_docstring__ = 'Autogenerated by Cheetah: The Python-Powered Template Engine'

if __CHEETAH_versionTuple__ < RequiredCheetahVersionTuple:
    raise AssertionError(
      'This template was compiled with Cheetah version'
      ' %s. Templates compiled before version %s must be recompiled.'%(
         __CHEETAH_version__, RequiredCheetahVersion))

##################################################
## CLASSES

class nodeMain(Template):

    ##################################################
    ## CHEETAH GENERATED METHODS


    def __init__(self, *args, **KWs):

        super(nodeMain, self).__init__(*args, **KWs)
        if not self._CHEETAH__instanceInitialized:
            cheetahKWArgs = {}
            allowedKWs = 'searchList namespaces filter filtersLib errorCatcher'.split()
            for k,v in KWs.items():
                if k in allowedKWs: cheetahKWArgs[k] = v
            self._initCheetahInstance(**cheetahKWArgs)
        

    def respond(self, trans=None):



        ## CHEETAH: main method generated for this template
        if (not trans and not self._CHEETAH__isBuffering and not callable(self.transaction)):
            trans = self.transaction # is None unless self.awake() was called
        if not trans:
            trans = DummyTransaction()
            _dummyTrans = True
        else: _dummyTrans = False
        write = trans.response().write
        SL = self._CHEETAH__searchList
        _filter = self._CHEETAH__currentFilter
        
        ########################################
        ## START - generated method body
        
        _v = VFSL([locals()]+SL+[globals(), builtin],"hash_include",True) # u'$hash_include' on line 1, col 1
        if _v is not None: write(_filter(_v, rawExpr=u'$hash_include')) # from line 1, col 1.
        write(u''' <cstdlib>
''')
        _v = VFSL([locals()]+SL+[globals(), builtin],"hash_include",True) # u'$hash_include' on line 2, col 1
        if _v is not None: write(_filter(_v, rawExpr=u'$hash_include')) # from line 2, col 1.
        write(u''' <string.h>
''')
        _v = VFSL([locals()]+SL+[globals(), builtin],"hash_include",True) # u'$hash_include' on line 3, col 1
        if _v is not None: write(_filter(_v, rawExpr=u'$hash_include')) # from line 3, col 1.
        write(u''' <dlfcn.h>
''')
        _v = VFSL([locals()]+SL+[globals(), builtin],"hash_include",True) # u'$hash_include' on line 4, col 1
        if _v is not None: write(_filter(_v, rawExpr=u'$hash_include')) # from line 4, col 1.
        write(u''' <exception>      
''')
        _v = VFSL([locals()]+SL+[globals(), builtin],"hash_include",True) # u'$hash_include' on line 5, col 1
        if _v is not None: write(_filter(_v, rawExpr=u'$hash_include')) # from line 5, col 1.
        write(u''' <cstdlib>        
''')
        _v = VFSL([locals()]+SL+[globals(), builtin],"hash_include",True) # u'$hash_include' on line 6, col 1
        if _v is not None: write(_filter(_v, rawExpr=u'$hash_include')) # from line 6, col 1.
        write(u''' <signal.h>
''')
        _v = VFSL([locals()]+SL+[globals(), builtin],"hash_include",True) # u'$hash_include' on line 7, col 1
        if _v is not None: write(_filter(_v, rawExpr=u'$hash_include')) # from line 7, col 1.
        write(u''' <boost/thread.hpp>
''')
        _v = VFSL([locals()]+SL+[globals(), builtin],"hash_include",True) # u'$hash_include' on line 8, col 1
        if _v is not None: write(_filter(_v, rawExpr=u'$hash_include')) # from line 8, col 1.
        write(u''' "node/Component.hpp"
''')
        _v = VFSL([locals()]+SL+[globals(), builtin],"hash_include",True) # u'$hash_include' on line 9, col 1
        if _v is not None: write(_filter(_v, rawExpr=u'$hash_include')) # from line 9, col 1.
        write(u''' "pthread.h"
''')
        _v = VFSL([locals()]+SL+[globals(), builtin],"hash_include",True) # u'$hash_include' on line 10, col 1
        if _v is not None: write(_filter(_v, rawExpr=u'$hash_include')) # from line 10, col 1.
        write(u''' "sched.h"

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

std::vector<boost::thread*> compThreads;\t

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
    // We\'ll set the priority to the maximum.     
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
      std::cout << "Couldn\'t retrieve real-time scheduling paramers" << std::endl;      
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
\tstd::cerr << dlerror() << std::endl;
\texit(-1);
      }
      else
\tROS_INFO_STREAM("Opened " << nodeParser.libList[i]);
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
\tstd::cerr << dlerror() << std::endl;
\texit(-1);
      }
      void *mkr = dlsym(hndl, "maker");
      Component *comp_inst = ((Component *(*)(ComponentConfig &, int , char **))(mkr))
\t(nodeParser.compConfigList[i], argc, argv);

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
    printf("ERROR::Unable to parse XML file\\n");
    return -1;
  }
}

''')
        
        ########################################
        ## END - generated method body
        
        return _dummyTrans and trans.response().getvalue() or ""
        
    ##################################################
    ## CHEETAH GENERATED ATTRIBUTES


    _CHEETAH__instanceInitialized = False

    _CHEETAH_version = __CHEETAH_version__

    _CHEETAH_versionTuple = __CHEETAH_versionTuple__

    _CHEETAH_genTime = __CHEETAH_genTime__

    _CHEETAH_genTimestamp = __CHEETAH_genTimestamp__

    _CHEETAH_src = __CHEETAH_src__

    _CHEETAH_srcLastModified = __CHEETAH_srcLastModified__

    _mainCheetahMethod_for_nodeMain= 'respond'

## END CLASS DEFINITION

if not hasattr(nodeMain, '_initCheetahAttributes'):
    templateAPIClass = getattr(nodeMain, '_CHEETAH_templateClass', Template)
    templateAPIClass._addCheetahPlumbingCodeToClass(nodeMain)


# CHEETAH was developed by Tavis Rudd and Mike Orr
# with code, advice and input from many other volunteers.
# For more information visit http://www.CheetahTemplate.org/

##################################################
## if run from command line:
if __name__ == '__main__':
    from Cheetah.TemplateCmdLineIface import CmdLineIface
    CmdLineIface(templateObj=nodeMain()).run()

