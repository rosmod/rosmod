cmake_minimum_required(VERSION 2.8.3)
project(rosmod)

find_package(catkin REQUIRED COMPONENTS message_generation roscpp rosunit std_msgs)
find_package(Boost REQUIRED COMPONENTS thread)

include_directories(include ${catkin_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
link_directories(${catkin_LIBRARY_DIRS})

# generate_messages(DEPENDENCIES std_msgs)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES rosmod
  CATKIN_DEPENDS message_runtime roscpp std_msgs
  DEPENDS Boost
)

add_library(rosmod src/rosmod/cmq.cpp
                   src/rosmod/timer.cpp 
                   src/rosmod/internal_timer_manager.cpp
                   src/rosmod/service_manager.cpp
                   src/rosmod/publisher.cpp
                   src/rosmod/service_server.cpp
                   src/rosmod/topic_manager.cpp
                   src/rosmod/subscriber.cpp
                   src/rosmod/subscriber_link.cpp
                   src/rosmod/subscription.cpp
                   src/rosmod/publisher_link.cpp
                   src/rosmod/publication.cpp
                   src/rosmod/single_subscriber_publisher.cpp
                   src/rosmod/intraprocess_subscriber_link.cpp
                   src/rosmod/intraprocess_publisher_link.cpp
                   src/rosmod/transport_publisher_link.cpp
                   src/rosmod/transport_subscriber_link.cpp
                   src/rosmod/node_handle.cpp)
target_link_libraries(rosmod ${catkin_LIBRARIES} ${Boost_LIBRARIES})
add_dependencies(rosmod rosmod_gencpp)

install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION})
install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h")

#if(CATKIN_ENABLE_TESTING)
#  find_package(rostest)
#  add_subdirectory(test)
# endif()
