cmake_minimum_required(VERSION 2.8.3)
project(agse_package)

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
  armState.msg

  payloadBayOrientation.msg

  controlInputs.msg

  payloadBayPosition.msg

  sampleOrientation.msg

  payloadBayState.msg

  sampleDetectionImages.msg

  samplePosition.msg

  sampleState.msg

  payloadBayDetectionImages.msg

)

# Generate services in the 'srv' folder
add_service_files(
  FILES
  captureImage.srv

  gripperRotation.srv

  gripperPos.srv

  armRotation.srv

  payloadBayStateFromImage.srv

  verticalPos.srv

  radialPos.srv

  sampleStateFromImage.srv

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
#  LIBRARIES agse_package
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
# add_library(agse_package
#   src/${PROJECT_NAME}/agse_package.cpp
# )

## Declare a cpp executable
# add_executable(agse_package_node src/agse_package_node.cpp)

## Add cmake target dependencies of the executable/library
## as an example, message headers may need to be generated before nodes
# add_dependencies(agse_package_node agse_package_generate_messages_cpp)

## Specify libraries to link a library or executable target against
# target_link_libraries(agse_package_node
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
# install(TARGETS agse_package agse_package_node
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
# catkin_add_gtest(${PROJECT_NAME}-test test/test_agse_package.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
include_directories(include ${catkin_INCLUDE_DIRS})

add_library(radial_actuator_controller
            src/agse_package/radial_actuator_controller.cpp
            ## Start radial_actuator_controller CPP Marker

            ## End radial_actuator_controller CPP Marker
           )
target_link_libraries(radial_actuator_controller
                      ## Start radial_actuator_controller Target Link Libraries Marker

                      ## End radial_actuator_controller Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(radial_actuator_controller
                 agse_package_generate_messages_cpp
                )

add_library(vertical_actuator_controller
            src/agse_package/vertical_actuator_controller.cpp
            ## Start vertical_actuator_controller CPP Marker

            ## End vertical_actuator_controller CPP Marker
           )
target_link_libraries(vertical_actuator_controller
                      ## Start vertical_actuator_controller Target Link Libraries Marker

                      ## End vertical_actuator_controller Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(vertical_actuator_controller
                 agse_package_generate_messages_cpp
                )

add_library(arm_controller
            src/agse_package/arm_controller.cpp
            ## Start arm_controller CPP Marker

            ## End arm_controller CPP Marker
           )
target_link_libraries(arm_controller
                      ## Start arm_controller Target Link Libraries Marker

                      ## End arm_controller Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(arm_controller
                 agse_package_generate_messages_cpp
                )

add_library(image_processor
            src/agse_package/image_processor.cpp
            ## Start image_processor CPP Marker

            ## End image_processor CPP Marker
           )
target_link_libraries(image_processor
                      ## Start image_processor Target Link Libraries Marker

                      ## End image_processor Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(image_processor
                 agse_package_generate_messages_cpp
                )

add_library(user_input_controller
            src/agse_package/user_input_controller.cpp
            ## Start user_input_controller CPP Marker

            ## End user_input_controller CPP Marker
           )
target_link_libraries(user_input_controller
                      ## Start user_input_controller Target Link Libraries Marker

                      ## End user_input_controller Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(user_input_controller
                 agse_package_generate_messages_cpp
                )

add_library(servo_controller
            src/agse_package/servo_controller.cpp
            ## Start servo_controller CPP Marker

            ## End servo_controller CPP Marker
           )
target_link_libraries(servo_controller
                      ## Start servo_controller Target Link Libraries Marker

                      ## End servo_controller Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(servo_controller
                 agse_package_generate_messages_cpp
                )

add_library(image_sensor
            src/agse_package/image_sensor.cpp
            ## Start image_sensor CPP Marker

            ## End image_sensor CPP Marker
           )
target_link_libraries(image_sensor
                      ## Start image_sensor Target Link Libraries Marker

                      ## End image_sensor Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(image_sensor
                 agse_package_generate_messages_cpp
                )

add_library(user_input_imager
            src/agse_package/user_input_imager.cpp
            ## Start user_input_imager CPP Marker

            ## End user_input_imager CPP Marker
           )
target_link_libraries(user_input_imager
                      ## Start user_input_imager Target Link Libraries Marker

                      ## End user_input_imager Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(user_input_imager
                 agse_package_generate_messages_cpp
                )

