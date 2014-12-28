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

add_library(rosmod src/rosmod/component_message_queue.cpp)
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
