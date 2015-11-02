cmake_minimum_required(VERSION 2.8.3)
project(tlc)

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
add_message_files(
  FILES
  ryg_control.msg

  sensor_state.msg

  ryg_state.msg

)

# Generate services in the 'srv' folder
add_service_files(
  FILES
  il_get_vehicle_number.srv

  e3_get_vehicle_ids.srv

  il_get_vehicle_ids.srv

  tlc_set_ryg_state.srv

  tlc_get_ryg_state.srv

  e3_get_vehicle_number.srv

)

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
#  LIBRARIES tlc
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
# add_library(tlc
#   src/${PROJECT_NAME}/tlc.cpp
# )

## Declare a cpp executable
# add_executable(tlc_node src/tlc_node.cpp)

## Add cmake target dependencies of the executable/library
## as an example, message headers may need to be generated before nodes
# add_dependencies(tlc_node tlc_generate_messages_cpp)

## Specify libraries to link a library or executable target against
# target_link_libraries(tlc_node
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
# install(TARGETS tlc tlc_node
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
# catkin_add_gtest(${PROJECT_NAME}-test test/test_tlc.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
include_directories(include ${catkin_INCLUDE_DIRS})

add_library(sumo_intf
            src/tlc/sumo_intf.cpp
            ## Start sumo_intf CPP Marker

            ## End sumo_intf CPP Marker
           )
target_link_libraries(sumo_intf
                      ## Start sumo_intf Target Link Libraries Marker

                      ## End sumo_intf Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(sumo_intf
                 tlc_generate_messages_cpp
                )

add_library(controller
            src/tlc/controller.cpp
            ## Start controller CPP Marker

            ## End controller CPP Marker
           )
target_link_libraries(controller
                      ## Start controller Target Link Libraries Marker

                      ## End controller Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(controller
                 tlc_generate_messages_cpp
                )

add_library(e3_sensor
            src/tlc/e3_sensor.cpp
            ## Start e3_sensor CPP Marker

            ## End e3_sensor CPP Marker
           )
target_link_libraries(e3_sensor
                      ## Start e3_sensor Target Link Libraries Marker

                      ## End e3_sensor Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(e3_sensor
                 tlc_generate_messages_cpp
                )

add_library(tl_actuator
            src/tlc/tl_actuator.cpp
            ## Start tl_actuator CPP Marker

            ## End tl_actuator CPP Marker
           )
target_link_libraries(tl_actuator
                      ## Start tl_actuator Target Link Libraries Marker

                      ## End tl_actuator Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(tl_actuator
                 tlc_generate_messages_cpp
                )

add_library(tl_sensor
            src/tlc/tl_sensor.cpp
            ## Start tl_sensor CPP Marker

            ## End tl_sensor CPP Marker
           )
target_link_libraries(tl_sensor
                      ## Start tl_sensor Target Link Libraries Marker

                      ## End tl_sensor Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(tl_sensor
                 tlc_generate_messages_cpp
                )

add_library(e3_sensor_stream_ddos
            src/tlc/e3_sensor_stream_ddos.cpp
            ## Start e3_sensor_stream_ddos CPP Marker

            ## End e3_sensor_stream_ddos CPP Marker
           )
target_link_libraries(e3_sensor_stream_ddos
                      ## Start e3_sensor_stream_ddos Target Link Libraries Marker

                      ## End e3_sensor_stream_ddos Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(e3_sensor_stream_ddos
                 tlc_generate_messages_cpp
                )

add_library(control_stream_ddos
            src/tlc/control_stream_ddos.cpp
            ## Start control_stream_ddos CPP Marker

            ## End control_stream_ddos CPP Marker
           )
target_link_libraries(control_stream_ddos
                      ## Start control_stream_ddos Target Link Libraries Marker

                      ## End control_stream_ddos Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(control_stream_ddos
                 tlc_generate_messages_cpp
                )

add_library(tl_sensor_stream_ddos
            src/tlc/tl_sensor_stream_ddos.cpp
            ## Start tl_sensor_stream_ddos CPP Marker

            ## End tl_sensor_stream_ddos CPP Marker
           )
target_link_libraries(tl_sensor_stream_ddos
                      ## Start tl_sensor_stream_ddos Target Link Libraries Marker

                      ## End tl_sensor_stream_ddos Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(tl_sensor_stream_ddos
                 tlc_generate_messages_cpp
                )

include_directories(include ${catkin_INCLUDE_DIRS}
	include/sumo
)
add_library(sumo
            src/sumo/sumo_client.cpp
	    include/sumo/utils/traci/TraCIAPI.cpp
	    include/sumo/foreign/tcpip/socket.cpp
	    include/sumo/foreign/tcpip/storage.cpp
	    )
