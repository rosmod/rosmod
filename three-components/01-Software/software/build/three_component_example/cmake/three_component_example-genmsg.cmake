# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "three_component_example: 1 messages, 1 services")

set(MSG_I_FLAGS "-Ithree_component_example:/home/kelsier/Repositories/rosmod-samples/three-components/01-Software/software/src/three_component_example/msg;-Istd_msgs:/opt/ros/indigo/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(three_component_example_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/kelsier/Repositories/rosmod-samples/three-components/01-Software/software/src/three_component_example/srv/ComponentService.srv" NAME_WE)
add_custom_target(_three_component_example_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "three_component_example" "/home/kelsier/Repositories/rosmod-samples/three-components/01-Software/software/src/three_component_example/srv/ComponentService.srv" ""
)

get_filename_component(_filename "/home/kelsier/Repositories/rosmod-samples/three-components/01-Software/software/src/three_component_example/msg/ComponentName.msg" NAME_WE)
add_custom_target(_three_component_example_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "three_component_example" "/home/kelsier/Repositories/rosmod-samples/three-components/01-Software/software/src/three_component_example/msg/ComponentName.msg" ""
)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(three_component_example
  "/home/kelsier/Repositories/rosmod-samples/three-components/01-Software/software/src/three_component_example/msg/ComponentName.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/three_component_example
)

### Generating Services
_generate_srv_cpp(three_component_example
  "/home/kelsier/Repositories/rosmod-samples/three-components/01-Software/software/src/three_component_example/srv/ComponentService.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/three_component_example
)

### Generating Module File
_generate_module_cpp(three_component_example
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/three_component_example
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(three_component_example_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(three_component_example_generate_messages three_component_example_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kelsier/Repositories/rosmod-samples/three-components/01-Software/software/src/three_component_example/srv/ComponentService.srv" NAME_WE)
add_dependencies(three_component_example_generate_messages_cpp _three_component_example_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kelsier/Repositories/rosmod-samples/three-components/01-Software/software/src/three_component_example/msg/ComponentName.msg" NAME_WE)
add_dependencies(three_component_example_generate_messages_cpp _three_component_example_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(three_component_example_gencpp)
add_dependencies(three_component_example_gencpp three_component_example_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS three_component_example_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(three_component_example
  "/home/kelsier/Repositories/rosmod-samples/three-components/01-Software/software/src/three_component_example/msg/ComponentName.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/three_component_example
)

### Generating Services
_generate_srv_lisp(three_component_example
  "/home/kelsier/Repositories/rosmod-samples/three-components/01-Software/software/src/three_component_example/srv/ComponentService.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/three_component_example
)

### Generating Module File
_generate_module_lisp(three_component_example
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/three_component_example
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(three_component_example_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(three_component_example_generate_messages three_component_example_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kelsier/Repositories/rosmod-samples/three-components/01-Software/software/src/three_component_example/srv/ComponentService.srv" NAME_WE)
add_dependencies(three_component_example_generate_messages_lisp _three_component_example_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kelsier/Repositories/rosmod-samples/three-components/01-Software/software/src/three_component_example/msg/ComponentName.msg" NAME_WE)
add_dependencies(three_component_example_generate_messages_lisp _three_component_example_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(three_component_example_genlisp)
add_dependencies(three_component_example_genlisp three_component_example_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS three_component_example_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(three_component_example
  "/home/kelsier/Repositories/rosmod-samples/three-components/01-Software/software/src/three_component_example/msg/ComponentName.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/three_component_example
)

### Generating Services
_generate_srv_py(three_component_example
  "/home/kelsier/Repositories/rosmod-samples/three-components/01-Software/software/src/three_component_example/srv/ComponentService.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/three_component_example
)

### Generating Module File
_generate_module_py(three_component_example
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/three_component_example
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(three_component_example_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(three_component_example_generate_messages three_component_example_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/kelsier/Repositories/rosmod-samples/three-components/01-Software/software/src/three_component_example/srv/ComponentService.srv" NAME_WE)
add_dependencies(three_component_example_generate_messages_py _three_component_example_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/kelsier/Repositories/rosmod-samples/three-components/01-Software/software/src/three_component_example/msg/ComponentName.msg" NAME_WE)
add_dependencies(three_component_example_generate_messages_py _three_component_example_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(three_component_example_genpy)
add_dependencies(three_component_example_genpy three_component_example_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS three_component_example_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/three_component_example)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/three_component_example
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(three_component_example_generate_messages_cpp std_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/three_component_example)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/three_component_example
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(three_component_example_generate_messages_lisp std_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/three_component_example)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/three_component_example\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/three_component_example
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
add_dependencies(three_component_example_generate_messages_py std_msgs_generate_messages_py)
