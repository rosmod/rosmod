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
__CHEETAH_genTime__ = 1458422848.832681
__CHEETAH_genTimestamp__ = 'Sat Mar 19 16:27:28 2016'
__CHEETAH_src__ = '/home/jeb/Repositories/rosmod/gui/templates/node_CMakeLists.tmpl'
__CHEETAH_srcLastModified__ = 'Tue Mar  8 10:57:08 2016'
__CHEETAH_docstring__ = 'Autogenerated by Cheetah: The Python-Powered Template Engine'

if __CHEETAH_versionTuple__ < RequiredCheetahVersionTuple:
    raise AssertionError(
      'This template was compiled with Cheetah version'
      ' %s. Templates compiled before version %s must be recompiled.'%(
         __CHEETAH_version__, RequiredCheetahVersion))

##################################################
## CLASSES

class node_CMakeLists(Template):

    ##################################################
    ## CHEETAH GENERATED METHODS


    def __init__(self, *args, **KWs):

        super(node_CMakeLists, self).__init__(*args, **KWs)
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
        
        write(u'''cmake_minimum_required(VERSION 2.8.3)
project(''')
        _v = VFFSL(SL,"package_name",True) # u'$package_name' on line 2, col 9
        if _v is not None: write(_filter(_v, rawExpr=u'$package_name')) # from line 2, col 9.
        write(u''')

## Check C++11 / C++0x
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "-std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "-std=c++0x")
else()
    message(FATAL_ERROR "The compiler ''')
        _v = VFFSL(SL,"CMAKE_CXX_COMPILER",True) # u'${CMAKE_CXX_COMPILER}' on line 13, col 39
        if _v is not None: write(_filter(_v, rawExpr=u'${CMAKE_CXX_COMPILER}')) # from line 13, col 39.
        write(u''' has no C++11 support. Please use a different C++ compiler.")
endif()


ADD_DEFINITIONS(-DNAMESPACE=''')
        _v = VFFSL(SL,"NAMESPACE",True) # u'${NAMESPACE}' on line 17, col 29
        if _v is not None: write(_filter(_v, rawExpr=u'${NAMESPACE}')) # from line 17, col 29.
        write(u''')
MESSAGE(STATUS ''')
        _v = VFFSL(SL,"NAMESPACE",True) # u'${NAMESPACE}' on line 18, col 16
        if _v is not None: write(_filter(_v, rawExpr=u'${NAMESPACE}')) # from line 18, col 16.
        write(u''')
if (''')
        _v = VFFSL(SL,"NAMESPACE",True) # u'${NAMESPACE}' on line 19, col 5
        if _v is not None: write(_filter(_v, rawExpr=u'${NAMESPACE}')) # from line 19, col 5.
        write(u''' STREQUAL "rosmod")
  find_package(catkin REQUIRED COMPONENTS rosmod std_msgs)
  ADD_DEFINITIONS(-DUSE_ROSMOD)
ELSEIF(''')
        _v = VFFSL(SL,"NAMESPACE",True) # u'${NAMESPACE}' on line 22, col 8
        if _v is not None: write(_filter(_v, rawExpr=u'${NAMESPACE}')) # from line 22, col 8.
        write(u''' STREQUAL "ros")
  find_package(catkin REQUIRED COMPONENTS roscpp std_msgs)
  ADD_DEFINITIONS(-DUSE_ROSCPP)
ENDIF()

## System dependencies are found with CMake\'s conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

#
## Declare ROS messages, services and actions 
#

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend and a run_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn\'t empty the following dependencies might have been
##     pulled in transitively but can be declared for certainty nonetheless:
##     * add a build_depend tag for "message_generation"
##     * add a run_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

# Generate messages in the \'msg\' folder
#add_message_files(
#  FILES
#)

# Generate services in the \'srv\' folder
#add_service_files(
#  FILES
#)

## Generate actions in the \'action\' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

# Generate added messages and services with any dependencies listed here
#generate_messages(
#  DEPENDENCIES
#  std_msgs
#)

#
## catkin specific configuration 
#
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES ''')
        _v = VFFSL(SL,"package_name",True) # u'$package_name' on line 94, col 14
        if _v is not None: write(_filter(_v, rawExpr=u'$package_name')) # from line 94, col 14.
        write(u'''
#  CATKIN_DEPENDS roscpp std_msgs
  CATKIN_DEPENDS message_runtime
#  DEPENDS system_lib
)

#
## Build 
#

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  ''')
        _v = VFFSL(SL,"catkin_INCLUDE_DIRS",True) # u'${catkin_INCLUDE_DIRS}' on line 108, col 3
        if _v is not None: write(_filter(_v, rawExpr=u'${catkin_INCLUDE_DIRS}')) # from line 108, col 3.
        write(u'''
)

## Declare a cpp library
# add_library(''')
        _v = VFFSL(SL,"package_name",True) # u'$package_name' on line 112, col 15
        if _v is not None: write(_filter(_v, rawExpr=u'$package_name')) # from line 112, col 15.
        write(u'''
#   src/''')
        _v = VFFSL(SL,"PROJECT_NAME",True) # u'${PROJECT_NAME}' on line 113, col 9
        if _v is not None: write(_filter(_v, rawExpr=u'${PROJECT_NAME}')) # from line 113, col 9.
        write(u'''/''')
        _v = VFFSL(SL,"package_name",True) # u'${package_name}' on line 113, col 25
        if _v is not None: write(_filter(_v, rawExpr=u'${package_name}')) # from line 113, col 25.
        write(u'''.cpp
# )

## Declare a cpp executable
# add_executable(''')
        _v = VFFSL(SL,"package_name",True) # u'${package_name}' on line 117, col 18
        if _v is not None: write(_filter(_v, rawExpr=u'${package_name}')) # from line 117, col 18.
        write(u'''_node src/''')
        _v = VFFSL(SL,"package_name",True) # u'${package_name}' on line 117, col 43
        if _v is not None: write(_filter(_v, rawExpr=u'${package_name}')) # from line 117, col 43.
        write(u'''_node.cpp)

## Add cmake target dependencies of the executable/library
## as an example, message headers may need to be generated before nodes
# add_dependencies(''')
        _v = VFFSL(SL,"package_name",True) # u'${package_name}' on line 121, col 20
        if _v is not None: write(_filter(_v, rawExpr=u'${package_name}')) # from line 121, col 20.
        write(u'''_node ''')
        _v = VFFSL(SL,"package_name",True) # u'${package_name}' on line 121, col 41
        if _v is not None: write(_filter(_v, rawExpr=u'${package_name}')) # from line 121, col 41.
        write(u'''_generate_messages_cpp)

## Specify libraries to link a library or executable target against
# target_link_libraries(''')
        _v = VFFSL(SL,"package_name",True) # u'${package_name}' on line 124, col 25
        if _v is not None: write(_filter(_v, rawExpr=u'${package_name}')) # from line 124, col 25.
        write(u'''_node
#   ''')
        _v = VFFSL(SL,"catkin_LIBRARIES",True) # u'${catkin_LIBRARIES}' on line 125, col 5
        if _v is not None: write(_filter(_v, rawExpr=u'${catkin_LIBRARIES}')) # from line 125, col 5.
        write(u'''
# )

#
## Install 
#

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ''')
        _v = VFFSL(SL,"CATKIN_PACKAGE_BIN_DESTINATION",True) # u'${CATKIN_PACKAGE_BIN_DESTINATION}' on line 139, col 17
        if _v is not None: write(_filter(_v, rawExpr=u'${CATKIN_PACKAGE_BIN_DESTINATION}')) # from line 139, col 17.
        write(u'''
# )

## Mark executables and/or libraries for installation
# install(TARGETS ''')
        _v = VFFSL(SL,"package_name",True) # u'$package_name' on line 143, col 19
        if _v is not None: write(_filter(_v, rawExpr=u'$package_name')) # from line 143, col 19.
        write(u''' ''')
        _v = VFFSL(SL,"package_name",True) # u'${package_name}' on line 143, col 33
        if _v is not None: write(_filter(_v, rawExpr=u'${package_name}')) # from line 143, col 33.
        write(u'''_node
#   ARCHIVE DESTINATION ''')
        _v = VFFSL(SL,"CATKIN_PACKAGE_LIB_DESTINATION",True) # u'${CATKIN_PACKAGE_LIB_DESTINATION}' on line 144, col 25
        if _v is not None: write(_filter(_v, rawExpr=u'${CATKIN_PACKAGE_LIB_DESTINATION}')) # from line 144, col 25.
        write(u'''
#   LIBRARY DESTINATION ''')
        _v = VFFSL(SL,"CATKIN_PACKAGE_LIB_DESTINATION",True) # u'${CATKIN_PACKAGE_LIB_DESTINATION}' on line 145, col 25
        if _v is not None: write(_filter(_v, rawExpr=u'${CATKIN_PACKAGE_LIB_DESTINATION}')) # from line 145, col 25.
        write(u'''
#   RUNTIME DESTINATION ''')
        _v = VFFSL(SL,"CATKIN_PACKAGE_BIN_DESTINATION",True) # u'${CATKIN_PACKAGE_BIN_DESTINATION}' on line 146, col 25
        if _v is not None: write(_filter(_v, rawExpr=u'${CATKIN_PACKAGE_BIN_DESTINATION}')) # from line 146, col 25.
        write(u'''
# )

## Mark cpp header files for installation
# install(DIRECTORY include/''')
        _v = VFFSL(SL,"PROJECT_NAME",True) # u'${PROJECT_NAME}' on line 150, col 29
        if _v is not None: write(_filter(_v, rawExpr=u'${PROJECT_NAME}')) # from line 150, col 29.
        write(u'''/
#   DESTINATION ''')
        _v = VFFSL(SL,"CATKIN_PACKAGE_INCLUDE_DESTINATION",True) # u'${CATKIN_PACKAGE_INCLUDE_DESTINATION}' on line 151, col 17
        if _v is not None: write(_filter(_v, rawExpr=u'${CATKIN_PACKAGE_INCLUDE_DESTINATION}')) # from line 151, col 17.
        write(u'''
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ''')
        _v = VFFSL(SL,"CATKIN_PACKAGE_SHARE_DESTINATION",True) # u'${CATKIN_PACKAGE_SHARE_DESTINATION}' on line 160, col 17
        if _v is not None: write(_filter(_v, rawExpr=u'${CATKIN_PACKAGE_SHARE_DESTINATION}')) # from line 160, col 17.
        write(u'''
# )

#
## Testing 
#

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(''')
        _v = VFFSL(SL,"PROJECT_NAME",True) # u'${PROJECT_NAME}' on line 168, col 20
        if _v is not None: write(_filter(_v, rawExpr=u'${PROJECT_NAME}')) # from line 168, col 20.
        write(u'''-test test/test_''')
        _v = VFFSL(SL,"package_name",True) # u'${package_name}' on line 168, col 51
        if _v is not None: write(_filter(_v, rawExpr=u'${package_name}')) # from line 168, col 51.
        write(u'''.cpp)
# if(TARGET ''')
        _v = VFFSL(SL,"PROJECT_NAME",True) # u'${PROJECT_NAME}' on line 169, col 13
        if _v is not None: write(_filter(_v, rawExpr=u'${PROJECT_NAME}')) # from line 169, col 13.
        write(u'''-test)
#   target_link_libraries(''')
        _v = VFFSL(SL,"PROJECT_NAME",True) # u'${PROJECT_NAME}' on line 170, col 27
        if _v is not None: write(_filter(_v, rawExpr=u'${PROJECT_NAME}')) # from line 170, col 27.
        write(u'''-test ''')
        _v = VFFSL(SL,"PROJECT_NAME",True) # u'${PROJECT_NAME}' on line 170, col 48
        if _v is not None: write(_filter(_v, rawExpr=u'${PROJECT_NAME}')) # from line 170, col 48.
        write(u''')
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
include_directories(include ''')
        _v = VFFSL(SL,"catkin_INCLUDE_DIRS",True) # u'${catkin_INCLUDE_DIRS}' on line 175, col 29
        if _v is not None: write(_filter(_v, rawExpr=u'${catkin_INCLUDE_DIRS}')) # from line 175, col 29.
        write(u''')
add_executable(node_main
\t       src/node/node_main.cpp
               src/node/Component.cpp)
target_link_libraries(node_main dl ''')
        _v = VFFSL(SL,"catkin_LIBRARIES",True) # u'${catkin_LIBRARIES}' on line 179, col 36
        if _v is not None: write(_filter(_v, rawExpr=u'${catkin_LIBRARIES}')) # from line 179, col 36.
        write(u''')












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

    _mainCheetahMethod_for_node_CMakeLists= 'respond'

## END CLASS DEFINITION

if not hasattr(node_CMakeLists, '_initCheetahAttributes'):
    templateAPIClass = getattr(node_CMakeLists, '_CHEETAH_templateClass', Template)
    templateAPIClass._addCheetahPlumbingCodeToClass(node_CMakeLists)


# CHEETAH was developed by Tavis Rudd and Mike Orr
# with code, advice and input from many other volunteers.
# For more information visit http://www.CheetahTemplate.org/

##################################################
## if run from command line:
if __name__ == '__main__':
    from Cheetah.TemplateCmdLineIface import CmdLineIface
    CmdLineIface(templateObj=node_CMakeLists()).run()


