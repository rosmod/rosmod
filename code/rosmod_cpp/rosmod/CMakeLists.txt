cmake_minimum_required(VERSION 2.8.3)
project(rosmod)

find_package(catkin REQUIRED COMPONENTS
  cpp_common message_generation rosconsole roscpp_serialization roscpp_traits rosgraph_msgs rostime std_msgs xmlrpcpp
)

catkin_package_xml()
# split version in parts and pass to extra file
string(REPLACE "." ";" rosmod_VERSION_LIST "${rosmod_VERSION}")
list(LENGTH rosmod_VERSION_LIST _count)
if(NOT _count EQUAL 3)
  message(FATAL_ERROR "rosmod version '${rosmod_VERSION}' does not match 'MAJOR.MINOR.PATCH' pattern")
endif()
list(GET rosmod_VERSION_LIST 0 roscpp_VERSION_MAJOR)
list(GET rosmod_VERSION_LIST 1 roscpp_VERSION_MINOR)
list(GET rosmod_VERSION_LIST 2 roscpp_VERSION_PATCH)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/ros/common.h.in ${CATKIN_DEVEL_PREFIX}/${CATKIN_GLOBAL_INCLUDE_DESTINATION}/ros/common.h)

find_package(Boost REQUIRED COMPONENTS signals filesystem system)

include_directories(include ${CATKIN_DEVEL_PREFIX}/${CATKIN_GLOBAL_INCLUDE_DESTINATION}/ros ${catkin_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

add_message_files(
  DIRECTORY msg
  FILES Logger.msg
)

add_service_files(
  DIRECTORY srv
  FILES Empty.srv GetLoggers.srv SetLoggerLevel.srv
)

generate_messages()

set(PTHREAD_LIB "")
find_package(Threads)
if(CMAKE_THREAD_LIBS_INIT)
  string(LENGTH ${CMAKE_THREAD_LIBS_INIT} _length)
  if(_length GREATER 2)
    string(SUBSTRING ${CMAKE_THREAD_LIBS_INIT} 0 2 _prefix)
    if(${_prefix} STREQUAL "-l")
      math(EXPR _rest_len "${_length} - 2")
      string(SUBSTRING ${CMAKE_THREAD_LIBS_INIT} 2 ${_rest_len} PTHREAD_LIB)
    endif()
  endif()
endif()

catkin_package(
  INCLUDE_DIRS include ${CATKIN_DEVEL_PREFIX}/${CATKIN_GLOBAL_INCLUDE_DESTINATION}/ros
  LIBRARIES rosmod ${PTHREAD_LIB}
  CATKIN_DEPENDS cpp_common message_runtime rosconsole roscpp_serialization roscpp_traits rosgraph_msgs rostime std_msgs xmlrpcpp
  DEPENDS Boost
)

include(CheckIncludeFiles)
include(CheckFunctionExists)

# Not everybody has <ifaddrs.h> (e.g., embedded arm-linux)
CHECK_INCLUDE_FILES(ifaddrs.h HAVE_IFADDRS_H)
# Not everybody has trunc (e.g., Windows, embedded arm-linux)
CHECK_FUNCTION_EXISTS(trunc HAVE_TRUNC)

# Output test results to config.h
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/libros/rosmod_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(rosmod 
  src/libros/rosmod_master.cpp
  src/libros/rosmod_network.cpp
  src/libros/rosmod_subscriber.cpp
  src/libros/rosmod_common.cpp
  src/libros/rosmod_publisher_link.cpp
  src/libros/rosmod_service_publication.cpp
  src/libros/rosmod_connection.cpp
  src/libros/rosmod_single_subscriber_publisher.cpp
  src/libros/rosmod_param.cpp
  src/libros/rosmod_service_server.cpp
  src/libros/rosmod_wall_timer.cpp
  src/libros/rosmod_xmlrpc_manager.cpp
  src/libros/rosmod_publisher.cpp
  src/libros/rosmod_timer.cpp
  src/libros/rosmod_io.cpp
  src/libros/rosmod_names.cpp
  src/libros/rosmod_topic.cpp
  src/libros/rosmod_topic_manager.cpp
  src/libros/rosmod_poll_manager.cpp
  src/libros/rosmod_publication.cpp
  src/libros/rosmod_statistics.cpp
  src/libros/rosmod_intraprocess_subscriber_link.cpp
  src/libros/rosmod_intraprocess_publisher_link.cpp
  src/libros/rosmod_callback_queue.cpp
  src/libros/rosmod_service_server_link.cpp
  src/libros/rosmod_service_client.cpp
  src/libros/rosmod_node_handle.cpp
  src/libros/rosmod_connection_manager.cpp
  src/libros/rosmod_file_log.cpp
  src/libros/transport/rosmod_transport.cpp
  src/libros/transport/rosmod_transport_udp.cpp
  src/libros/transport/rosmod_transport_tcp.cpp
  src/libros/rosmod_subscriber_link.cpp
  src/libros/rosmod_service_client_link.cpp
  src/libros/rosmod_transport_publisher_link.cpp
  src/libros/rosmod_transport_subscriber_link.cpp
  src/libros/rosmod_service_manager.cpp
  src/libros/rosmod_rosout_appender.cpp
  src/libros/rosmod_init.cpp
  src/libros/rosmod_subscription.cpp
  src/libros/rosmod_subscription_queue.cpp
  src/libros/rosmod_spinner.cpp
  src/libros/rosmod_internal_timer_manager.cpp
  src/libros/rosmod_message_deserializer.cpp
  src/libros/rosmod_poll_set.cpp
  src/libros/rosmod_service.cpp
  src/libros/rosmod_this_node.cpp
  )

add_dependencies(rosmod roscpp_gencpp rosgraph_msgs_gencpp std_msgs_gencpp)

target_link_libraries(rosmod
  ${catkin_LIBRARIES}
  ${Boost_LIBRARIES}
  )

#explicitly install library and includes
install(TARGETS rosmod
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION})
install(DIRECTORY include/
  DESTINATION ${CATKIN_GLOBAL_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h")
install(FILES ${CATKIN_DEVEL_PREFIX}/${CATKIN_GLOBAL_INCLUDE_DESTINATION}/ros/common.h
  DESTINATION ${CATKIN_GLOBAL_INCLUDE_DESTINATION}/ros)
# install legacy infrastructure needed by rosbuild
install(FILES rosbuild/rosmod.cmake
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/rosbuild)
catkin_install_python(PROGRAMS
  rosbuild/scripts/genmsg_cpp.py
  rosbuild/scripts/gensrv_cpp.py
  rosbuild/scripts/msg_gen.py
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/rosbuild/scripts)
