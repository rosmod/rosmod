# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "trajectory_planning_package: 1 messages, 1 services")

set(MSG_I_FLAGS "-Itrajectory_planning_package:/home/kelsier/Repositories/rosmod-samples/trajectory-planner/01-Software/software/src/trajectory_planning_package/msg;-Istd_msgs:/opt/ros/indigo/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(trajectory_planning_package_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/kelsier/Repositories/rosmod-samples/trajectory-planner/01-Software/software/src/trajectory_planning_package/msg/sensor_reading.msg" NAME_WE)
add_custom_target(_trajectory_planning_package_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "trajectory_planning_package" "/home/kelsier/Repositories/rosmod-samples/trajectory-planner/01-Software/software/src/trajectory_planning_package/msg/sensor_reading.msg" ""
)

get_filename_component(_filename "/home/kelsier/Repositories/rosmod-samples/trajectory-planner/01-Software/software/src/trajectory_planning_package/srv/compute.srv" NAME_WE)
add_custom_target(_trajectory_planning_package_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "trajectory_planning_package" "/home/kelsier/Repositories/rosmod-samples/trajectory-planner/01-Software/software/src/trajectory_planning_package/srv/compute.srv" ""
)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(trajectory_planning_package
  "/home/kelsier/Repositories/rosmod-samples/trajectory-planner/01-Software/software/src/trajectory_planning_package/msg/sensor_reading.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/trajectory_planning_package
)

### Generating Services
_generate_srv_cpp(trajectory_planning_package
  "/home/kelsier/Repositories/rosmod-samples/trajectory-planner/01-Software/software/src/trajectory_planning_package/srv/compute.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/trajectory_planning_package
)

### Generating Module File
_generate_module_cpp(trajectory_planning_package
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/trajectory_planning_package
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(trajectory_planning_package_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(trajectory_planning_package_generate_messages trajectory_planning_package_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kelsier/Repositories/rosmod-samples/trajectory-planner/01-Software/software/src/trajectory_planning_package/msg/sensor_reading.msg" NAME_WE)
add_dependencies(trajectory_planning_package_generate_messages_cpp _trajectory_planning_package_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kelsier/Repositories/rosmod-samples/trajectory-planner/01-Software/software/src/trajectory_planning_package/srv/compute.srv" NAME_WE)
add_dependencies(trajectory_planning_package_generate_messages_cpp _trajectory_planning_package_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(trajectory_planning_package_gencpp)
add_dependencies(trajectory_planning_package_gencpp trajectory_planning_package_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS trajectory_planning_package_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(trajectory_planning_package
  "/home/kelsier/Repositories/rosmod-samples/trajectory-planner/01-Software/software/src/trajectory_planning_package/msg/sensor_reading.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/trajectory_planning_package
)

### Generating Services
_generate_srv_lisp(trajectory_planning_package
  "/home/kelsier/Repositories/rosmod-samples/trajectory-planner/01-Software/software/src/trajectory_planning_package/srv/compute.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/trajectory_planning_package
)

### Generating Module File
_generate_module_lisp(trajectory_planning_package
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/trajectory_planning_package
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(trajectory_planning_package_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(trajectory_planning_package_generate_messages trajectory_planning_package_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kelsier/Repositories/rosmod-samples/trajectory-planner/01-Software/software/src/trajectory_planning_package/msg/sensor_reading.msg" NAME_WE)
add_dependencies(trajectory_planning_package_generate_messages_lisp _trajectory_planning_package_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kelsier/Repositories/rosmod-samples/trajectory-planner/01-Software/software/src/trajectory_planning_package/srv/compute.srv" NAME_WE)
add_dependencies(trajectory_planning_package_generate_messages_lisp _trajectory_planning_package_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(trajectory_planning_package_genlisp)
add_dependencies(trajectory_planning_package_genlisp trajectory_planning_package_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS trajectory_planning_package_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(trajectory_planning_package
  "/home/kelsier/Repositories/rosmod-samples/trajectory-planner/01-Software/software/src/trajectory_planning_package/msg/sensor_reading.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/trajectory_planning_package
)

### Generating Services
_generate_srv_py(trajectory_planning_package
  "/home/kelsier/Repositories/rosmod-samples/trajectory-planner/01-Software/software/src/trajectory_planning_package/srv/compute.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/trajectory_planning_package
)

### Generating Module File
_generate_module_py(trajectory_planning_package
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/trajectory_planning_package
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(trajectory_planning_package_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(trajectory_planning_package_generate_messages trajectory_planning_package_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kelsier/Repositories/rosmod-samples/trajectory-planner/01-Software/software/src/trajectory_planning_package/msg/sensor_reading.msg" NAME_WE)
add_dependencies(trajectory_planning_package_generate_messages_py _trajectory_planning_package_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kelsier/Repositories/rosmod-samples/trajectory-planner/01-Software/software/src/trajectory_planning_package/srv/compute.srv" NAME_WE)
add_dependencies(trajectory_planning_package_generate_messages_py _trajectory_planning_package_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(trajectory_planning_package_genpy)
add_dependencies(trajectory_planning_package_genpy trajectory_planning_package_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS trajectory_planning_package_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/trajectory_planning_package)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/trajectory_planning_package
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(trajectory_planning_package_generate_messages_cpp std_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/trajectory_planning_package)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/trajectory_planning_package
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(trajectory_planning_package_generate_messages_lisp std_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/trajectory_planning_package)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/trajectory_planning_package\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/trajectory_planning_package
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
add_dependencies(trajectory_planning_package_generate_messages_py std_msgs_generate_messages_py)
