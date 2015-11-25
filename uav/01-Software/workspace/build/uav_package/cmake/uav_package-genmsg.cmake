# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "uav_package: 2 messages, 0 services")

set(MSG_I_FLAGS "-Iuav_package:/home/kelsier/Repositories/cpn-timing-analysis/tests/uav/UAV_ROSMOD/01-Software/workspace/src/uav_package/msg;-Istd_msgs:/opt/ros/indigo/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(uav_package_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/kelsier/Repositories/cpn-timing-analysis/tests/uav/UAV_ROSMOD/01-Software/workspace/src/uav_package/msg/control_command.msg" NAME_WE)
add_custom_target(_uav_package_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "uav_package" "/home/kelsier/Repositories/cpn-timing-analysis/tests/uav/UAV_ROSMOD/01-Software/workspace/src/uav_package/msg/control_command.msg" ""
)

get_filename_component(_filename "/home/kelsier/Repositories/cpn-timing-analysis/tests/uav/UAV_ROSMOD/01-Software/workspace/src/uav_package/msg/sensor_state.msg" NAME_WE)
add_custom_target(_uav_package_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "uav_package" "/home/kelsier/Repositories/cpn-timing-analysis/tests/uav/UAV_ROSMOD/01-Software/workspace/src/uav_package/msg/sensor_state.msg" ""
)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(uav_package
  "/home/kelsier/Repositories/cpn-timing-analysis/tests/uav/UAV_ROSMOD/01-Software/workspace/src/uav_package/msg/control_command.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/uav_package
)
_generate_msg_cpp(uav_package
  "/home/kelsier/Repositories/cpn-timing-analysis/tests/uav/UAV_ROSMOD/01-Software/workspace/src/uav_package/msg/sensor_state.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/uav_package
)

### Generating Services

### Generating Module File
_generate_module_cpp(uav_package
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/uav_package
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(uav_package_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(uav_package_generate_messages uav_package_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kelsier/Repositories/cpn-timing-analysis/tests/uav/UAV_ROSMOD/01-Software/workspace/src/uav_package/msg/control_command.msg" NAME_WE)
add_dependencies(uav_package_generate_messages_cpp _uav_package_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kelsier/Repositories/cpn-timing-analysis/tests/uav/UAV_ROSMOD/01-Software/workspace/src/uav_package/msg/sensor_state.msg" NAME_WE)
add_dependencies(uav_package_generate_messages_cpp _uav_package_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(uav_package_gencpp)
add_dependencies(uav_package_gencpp uav_package_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS uav_package_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(uav_package
  "/home/kelsier/Repositories/cpn-timing-analysis/tests/uav/UAV_ROSMOD/01-Software/workspace/src/uav_package/msg/control_command.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/uav_package
)
_generate_msg_lisp(uav_package
  "/home/kelsier/Repositories/cpn-timing-analysis/tests/uav/UAV_ROSMOD/01-Software/workspace/src/uav_package/msg/sensor_state.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/uav_package
)

### Generating Services

### Generating Module File
_generate_module_lisp(uav_package
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/uav_package
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(uav_package_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(uav_package_generate_messages uav_package_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kelsier/Repositories/cpn-timing-analysis/tests/uav/UAV_ROSMOD/01-Software/workspace/src/uav_package/msg/control_command.msg" NAME_WE)
add_dependencies(uav_package_generate_messages_lisp _uav_package_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kelsier/Repositories/cpn-timing-analysis/tests/uav/UAV_ROSMOD/01-Software/workspace/src/uav_package/msg/sensor_state.msg" NAME_WE)
add_dependencies(uav_package_generate_messages_lisp _uav_package_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(uav_package_genlisp)
add_dependencies(uav_package_genlisp uav_package_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS uav_package_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(uav_package
  "/home/kelsier/Repositories/cpn-timing-analysis/tests/uav/UAV_ROSMOD/01-Software/workspace/src/uav_package/msg/control_command.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/uav_package
)
_generate_msg_py(uav_package
  "/home/kelsier/Repositories/cpn-timing-analysis/tests/uav/UAV_ROSMOD/01-Software/workspace/src/uav_package/msg/sensor_state.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/uav_package
)

### Generating Services

### Generating Module File
_generate_module_py(uav_package
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/uav_package
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(uav_package_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(uav_package_generate_messages uav_package_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kelsier/Repositories/cpn-timing-analysis/tests/uav/UAV_ROSMOD/01-Software/workspace/src/uav_package/msg/control_command.msg" NAME_WE)
add_dependencies(uav_package_generate_messages_py _uav_package_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kelsier/Repositories/cpn-timing-analysis/tests/uav/UAV_ROSMOD/01-Software/workspace/src/uav_package/msg/sensor_state.msg" NAME_WE)
add_dependencies(uav_package_generate_messages_py _uav_package_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(uav_package_genpy)
add_dependencies(uav_package_genpy uav_package_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS uav_package_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/uav_package)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/uav_package
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(uav_package_generate_messages_cpp std_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/uav_package)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/uav_package
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(uav_package_generate_messages_lisp std_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/uav_package)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/uav_package\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/uav_package
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
add_dependencies(uav_package_generate_messages_py std_msgs_generate_messages_py)
