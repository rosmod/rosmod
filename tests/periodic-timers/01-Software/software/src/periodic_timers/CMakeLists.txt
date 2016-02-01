cmake_minimum_required(VERSION 2.8.3)
project(periodic_timers)

## Start Global Marker

## End Global Marker

## Check C++11 / C++0x
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "-std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "-std=c++0x")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

ADD_DEFINITIONS(-DNAMESPACE=${NAMESPACE})
if (${NAMESPACE} STREQUAL "rosmod")
  find_package(catkin REQUIRED COMPONENTS rosmod std_msgs message_generation)
  ADD_DEFINITIONS(-DUSE_ROSMOD)
ELSEIF(${NAMESPACE} STREQUAL "ros")
  find_package(catkin REQUIRED COMPONENTS roscpp std_msgs message_generation)
  ADD_DEFINITIONS(-DUSE_ROSCPP)
ELSE()
  message(FATAL_ERROR "Some error something wrong")
ENDIF()

## System dependencies are found with CMake's conventions
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
##   * If MSG_DEP_SET isn't empty the following dependencies might have been
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

# Generate messages in the 'msg' folder

# Generate services in the 'srv' folder

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

# Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
)

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
#  LIBRARIES periodic_timers
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
  ../node/include
  ${catkin_INCLUDE_DIRS}
## Start Include Directories Marker
## End Include Directories Marker
)

## Declare a cpp library
# add_library(periodic_timers
#   src/${PROJECT_NAME}/periodic_timers.cpp
# )

## Declare a cpp executable
# add_executable(periodic_timers_node src/periodic_timers_node.cpp)

## Add cmake target dependencies of the executable/library
## as an example, message headers may need to be generated before nodes
# add_dependencies(periodic_timers_node periodic_timers_generate_messages_cpp)

## Specify libraries to link a library or executable target against
# target_link_libraries(periodic_timers_node
#   ${catkin_LIBRARIES}
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
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS periodic_timers periodic_timers_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#
## Testing 
#

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_periodic_timers.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
include_directories(include ${catkin_INCLUDE_DIRS})

add_library(Component_1
            src/periodic_timers/Component_1.cpp
            ## Start Component_1 CPP Marker

            ## End Component_1 CPP Marker
           )
target_link_libraries(Component_1
                      ## Start Component_1 Target Link Libraries Marker

                      ## End Component_1 Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(Component_1
                 periodic_timers_generate_messages_cpp
                )

add_library(Component_2
            src/periodic_timers/Component_2.cpp
            ## Start Component_2 CPP Marker

            ## End Component_2 CPP Marker
           )
target_link_libraries(Component_2
                      ## Start Component_2 Target Link Libraries Marker

                      ## End Component_2 Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(Component_2
                 periodic_timers_generate_messages_cpp
                )

add_library(Component_3
            src/periodic_timers/Component_3.cpp
            ## Start Component_3 CPP Marker

            ## End Component_3 CPP Marker
           )
target_link_libraries(Component_3
                      ## Start Component_3 Target Link Libraries Marker

                      ## End Component_3 Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(Component_3
                 periodic_timers_generate_messages_cpp
                )

add_library(Component_4
            src/periodic_timers/Component_4.cpp
            ## Start Component_4 CPP Marker

            ## End Component_4 CPP Marker
           )
target_link_libraries(Component_4
                      ## Start Component_4 Target Link Libraries Marker

                      ## End Component_4 Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(Component_4
                 periodic_timers_generate_messages_cpp
                )

add_library(Component_5
            src/periodic_timers/Component_5.cpp
            ## Start Component_5 CPP Marker

            ## End Component_5 CPP Marker
           )
target_link_libraries(Component_5
                      ## Start Component_5 Target Link Libraries Marker

                      ## End Component_5 Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(Component_5
                 periodic_timers_generate_messages_cpp
                )

add_library(All_Timers
            src/periodic_timers/All_Timers.cpp
            ## Start All_Timers CPP Marker

            ## End All_Timers CPP Marker
           )
target_link_libraries(All_Timers
                      ## Start All_Timers Target Link Libraries Marker

                      ## End All_Timers Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(All_Timers
                 periodic_timers_generate_messages_cpp
                )

add_library(Two_Timers
            src/periodic_timers/Two_Timers.cpp
            ## Start Two_Timers CPP Marker

            ## End Two_Timers CPP Marker
           )
target_link_libraries(Two_Timers
                      ## Start Two_Timers Target Link Libraries Marker

                      ## End Two_Timers Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(Two_Timers
                 periodic_timers_generate_messages_cpp
                )

add_library(Three_Timers
            src/periodic_timers/Three_Timers.cpp
            ## Start Three_Timers CPP Marker

            ## End Three_Timers CPP Marker
           )
target_link_libraries(Three_Timers
                      ## Start Three_Timers Target Link Libraries Marker

                      ## End Three_Timers Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(Three_Timers
                 periodic_timers_generate_messages_cpp
                )

