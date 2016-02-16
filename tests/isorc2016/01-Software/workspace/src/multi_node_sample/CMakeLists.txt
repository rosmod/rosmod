cmake_minimum_required(VERSION 2.8.3)
project(multi_node_sample)

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
  messageE.msg

  messageB.msg

  messageC.msg

  messageA.msg

  messageD.msg

)

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
#  LIBRARIES multi_node_sample
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
# add_library(multi_node_sample
#   src/${PROJECT_NAME}/multi_node_sample.cpp
# )

## Declare a cpp executable
# add_executable(multi_node_sample_node src/multi_node_sample_node.cpp)

## Add cmake target dependencies of the executable/library
## as an example, message headers may need to be generated before nodes
# add_dependencies(multi_node_sample_node multi_node_sample_generate_messages_cpp)

## Specify libraries to link a library or executable target against
# target_link_libraries(multi_node_sample_node
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
# install(TARGETS multi_node_sample multi_node_sample_node
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
# catkin_add_gtest(${PROJECT_NAME}-test test/test_multi_node_sample.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
include_directories(include ${catkin_INCLUDE_DIRS})

add_library(senderBCDE
            src/multi_node_sample/senderBCDE.cpp
            ## Start senderBCDE CPP Marker

            ## End senderBCDE CPP Marker
           )
target_link_libraries(senderBCDE
                      ## Start senderBCDE Target Link Libraries Marker

                      ## End senderBCDE Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(senderBCDE
                 multi_node_sample_generate_messages_cpp
                )

add_library(senderACDE
            src/multi_node_sample/senderACDE.cpp
            ## Start senderACDE CPP Marker

            ## End senderACDE CPP Marker
           )
target_link_libraries(senderACDE
                      ## Start senderACDE Target Link Libraries Marker

                      ## End senderACDE Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(senderACDE
                 multi_node_sample_generate_messages_cpp
                )

add_library(senderABDE
            src/multi_node_sample/senderABDE.cpp
            ## Start senderABDE CPP Marker

            ## End senderABDE CPP Marker
           )
target_link_libraries(senderABDE
                      ## Start senderABDE Target Link Libraries Marker

                      ## End senderABDE Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(senderABDE
                 multi_node_sample_generate_messages_cpp
                )

add_library(senderABCE
            src/multi_node_sample/senderABCE.cpp
            ## Start senderABCE CPP Marker

            ## End senderABCE CPP Marker
           )
target_link_libraries(senderABCE
                      ## Start senderABCE Target Link Libraries Marker

                      ## End senderABCE Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(senderABCE
                 multi_node_sample_generate_messages_cpp
                )

add_library(senderABCD
            src/multi_node_sample/senderABCD.cpp
            ## Start senderABCD CPP Marker

            ## End senderABCD CPP Marker
           )
target_link_libraries(senderABCD
                      ## Start senderABCD Target Link Libraries Marker

                      ## End senderABCD Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(senderABCD
                 multi_node_sample_generate_messages_cpp
                )

add_library(receiverA
            src/multi_node_sample/receiverA.cpp
            ## Start receiverA CPP Marker

            ## End receiverA CPP Marker
           )
target_link_libraries(receiverA
                      ## Start receiverA Target Link Libraries Marker

                      ## End receiverA Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(receiverA
                 multi_node_sample_generate_messages_cpp
                )

add_library(receiverB
            src/multi_node_sample/receiverB.cpp
            ## Start receiverB CPP Marker

            ## End receiverB CPP Marker
           )
target_link_libraries(receiverB
                      ## Start receiverB Target Link Libraries Marker

                      ## End receiverB Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(receiverB
                 multi_node_sample_generate_messages_cpp
                )

add_library(receiverC
            src/multi_node_sample/receiverC.cpp
            ## Start receiverC CPP Marker

            ## End receiverC CPP Marker
           )
target_link_libraries(receiverC
                      ## Start receiverC Target Link Libraries Marker

                      ## End receiverC Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(receiverC
                 multi_node_sample_generate_messages_cpp
                )

add_library(receiverD
            src/multi_node_sample/receiverD.cpp
            ## Start receiverD CPP Marker

            ## End receiverD CPP Marker
           )
target_link_libraries(receiverD
                      ## Start receiverD Target Link Libraries Marker

                      ## End receiverD Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(receiverD
                 multi_node_sample_generate_messages_cpp
                )

add_library(receiverE
            src/multi_node_sample/receiverE.cpp
            ## Start receiverE CPP Marker

            ## End receiverE CPP Marker
           )
target_link_libraries(receiverE
                      ## Start receiverE Target Link Libraries Marker

                      ## End receiverE Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(receiverE
                 multi_node_sample_generate_messages_cpp
                )

