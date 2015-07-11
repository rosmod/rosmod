cmake_minimum_required(VERSION 2.8.3)
project(flight_controller_package)

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

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
roscpp
std_msgs
message_generation
## Start Package Marker
## End Package Marker
)

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
  Roll.msg

  Latitude.msg

  Speed.msg

  Longitude.msg

  Yaw.msg

  Heading.msg

  Altitude.msg

  Pitch.msg

)

# Generate services in the 'srv' folder
add_service_files(
  FILES
  Throttle_Control.srv

  Yaw_Control.srv

  Roll_Control.srv

  Landing_Gear_Control.srv

  Heading_Control.srv

  Pitch_Control.srv

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
#  LIBRARIES flight_controller_package
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
  ${catkin_INCLUDE_DIRS}
## Start Include Directories Marker
## End Include Directories Marker
)

## Declare a cpp library
# add_library(flight_controller_package
#   src/${PROJECT_NAME}/flight_controller_package.cpp
# )

## Declare a cpp executable
# add_executable(flight_controller_package_node src/flight_controller_package_node.cpp)

## Add cmake target dependencies of the executable/library
## as an example, message headers may need to be generated before nodes
# add_dependencies(flight_controller_package_node flight_controller_package_generate_messages_cpp)

## Specify libraries to link a library or executable target against
# target_link_libraries(flight_controller_package_node
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
# install(TARGETS flight_controller_package flight_controller_package_node
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
# catkin_add_gtest(${PROJECT_NAME}-test test/test_flight_controller_package.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
include_directories(include ${catkin_INCLUDE_DIRS})

add_library(Pitch_Sensor
            src/flight_controller_package/Pitch_Sensor.cpp
            ## Start Pitch_Sensor CPP Marker

            ## End Pitch_Sensor CPP Marker
           )
target_link_libraries(Pitch_Sensor
                      ## Start Pitch_Sensor Target Link Libraries Marker

                      ## End Pitch_Sensor Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(Pitch_Sensor
                 flight_controller_package_generate_messages_cpp
                )

add_library(Heading_Sensor
            src/flight_controller_package/Heading_Sensor.cpp
            ## Start Heading_Sensor CPP Marker

            ## End Heading_Sensor CPP Marker
           )
target_link_libraries(Heading_Sensor
                      ## Start Heading_Sensor Target Link Libraries Marker

                      ## End Heading_Sensor Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(Heading_Sensor
                 flight_controller_package_generate_messages_cpp
                )

add_library(Roll_Sensor
            src/flight_controller_package/Roll_Sensor.cpp
            ## Start Roll_Sensor CPP Marker

            ## End Roll_Sensor CPP Marker
           )
target_link_libraries(Roll_Sensor
                      ## Start Roll_Sensor Target Link Libraries Marker

                      ## End Roll_Sensor Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(Roll_Sensor
                 flight_controller_package_generate_messages_cpp
                )

add_library(Altitude_Sensor
            src/flight_controller_package/Altitude_Sensor.cpp
            ## Start Altitude_Sensor CPP Marker

            ## End Altitude_Sensor CPP Marker
           )
target_link_libraries(Altitude_Sensor
                      ## Start Altitude_Sensor Target Link Libraries Marker

                      ## End Altitude_Sensor Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(Altitude_Sensor
                 flight_controller_package_generate_messages_cpp
                )

add_library(Pitch_Actuator
            src/flight_controller_package/Pitch_Actuator.cpp
            ## Start Pitch_Actuator CPP Marker

            ## End Pitch_Actuator CPP Marker
           )
target_link_libraries(Pitch_Actuator
                      ## Start Pitch_Actuator Target Link Libraries Marker

                      ## End Pitch_Actuator Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(Pitch_Actuator
                 flight_controller_package_generate_messages_cpp
                )

add_library(Roll_Actuator
            src/flight_controller_package/Roll_Actuator.cpp
            ## Start Roll_Actuator CPP Marker

            ## End Roll_Actuator CPP Marker
           )
target_link_libraries(Roll_Actuator
                      ## Start Roll_Actuator Target Link Libraries Marker

                      ## End Roll_Actuator Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(Roll_Actuator
                 flight_controller_package_generate_messages_cpp
                )

add_library(Heading_Actuator
            src/flight_controller_package/Heading_Actuator.cpp
            ## Start Heading_Actuator CPP Marker

            ## End Heading_Actuator CPP Marker
           )
target_link_libraries(Heading_Actuator
                      ## Start Heading_Actuator Target Link Libraries Marker

                      ## End Heading_Actuator Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(Heading_Actuator
                 flight_controller_package_generate_messages_cpp
                )

add_library(Throttle_Actuator
            src/flight_controller_package/Throttle_Actuator.cpp
            ## Start Throttle_Actuator CPP Marker

            ## End Throttle_Actuator CPP Marker
           )
target_link_libraries(Throttle_Actuator
                      ## Start Throttle_Actuator Target Link Libraries Marker

                      ## End Throttle_Actuator Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(Throttle_Actuator
                 flight_controller_package_generate_messages_cpp
                )

add_library(Landing_Gear_Controller
            src/flight_controller_package/Landing_Gear_Controller.cpp
            ## Start Landing_Gear_Controller CPP Marker

            ## End Landing_Gear_Controller CPP Marker
           )
target_link_libraries(Landing_Gear_Controller
                      ## Start Landing_Gear_Controller Target Link Libraries Marker

                      ## End Landing_Gear_Controller Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(Landing_Gear_Controller
                 flight_controller_package_generate_messages_cpp
                )

add_library(PID_Controller
            src/flight_controller_package/PID_Controller.cpp
            ## Start PID_Controller CPP Marker

            ## End PID_Controller CPP Marker
           )
target_link_libraries(PID_Controller
                      ## Start PID_Controller Target Link Libraries Marker

                      ## End PID_Controller Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(PID_Controller
                 flight_controller_package_generate_messages_cpp
                )

add_library(High_level_Controller
            src/flight_controller_package/High_level_Controller.cpp
            ## Start High_level_Controller CPP Marker

            ## End High_level_Controller CPP Marker
           )
target_link_libraries(High_level_Controller
                      ## Start High_level_Controller Target Link Libraries Marker

                      ## End High_level_Controller Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(High_level_Controller
                 flight_controller_package_generate_messages_cpp
                )

add_library(Speed_Sensor
            src/flight_controller_package/Speed_Sensor.cpp
            ## Start Speed_Sensor CPP Marker

            ## End Speed_Sensor CPP Marker
           )
target_link_libraries(Speed_Sensor
                      ## Start Speed_Sensor Target Link Libraries Marker

                      ## End Speed_Sensor Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(Speed_Sensor
                 flight_controller_package_generate_messages_cpp
                )

add_library(Latitude_Sensor
            src/flight_controller_package/Latitude_Sensor.cpp
            ## Start Latitude_Sensor CPP Marker

            ## End Latitude_Sensor CPP Marker
           )
target_link_libraries(Latitude_Sensor
                      ## Start Latitude_Sensor Target Link Libraries Marker

                      ## End Latitude_Sensor Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(Latitude_Sensor
                 flight_controller_package_generate_messages_cpp
                )

add_library(Longitude_Sensor
            src/flight_controller_package/Longitude_Sensor.cpp
            ## Start Longitude_Sensor CPP Marker

            ## End Longitude_Sensor CPP Marker
           )
target_link_libraries(Longitude_Sensor
                      ## Start Longitude_Sensor Target Link Libraries Marker

                      ## End Longitude_Sensor Target Link Libraries Marker
                      ${catkin_LIBRARIES}
                      )
add_dependencies(Longitude_Sensor
                 flight_controller_package_generate_messages_cpp
                )

add_library(KRPCI
            src/krpci/KRPC.pb.cc
	    src/krpci/krpci_base.cpp
            src/krpci/krpci_generated.cpp
	    )
target_link_libraries(KRPCI
  boost_system boost_thread /usr/lib/libprotobuf.so.10
  )
