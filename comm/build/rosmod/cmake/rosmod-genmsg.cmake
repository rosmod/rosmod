# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "rosmod: 1 messages, 3 services")

set(MSG_I_FLAGS "-Irosmod:/home/jeb/Repositories/rosmod/comm/src/rosmod/msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(rosmod_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/Empty.srv" NAME_WE)
add_custom_target(_rosmod_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "rosmod" "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/Empty.srv" ""
)

get_filename_component(_filename "/home/jeb/Repositories/rosmod/comm/src/rosmod/msg/Logger.msg" NAME_WE)
add_custom_target(_rosmod_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "rosmod" "/home/jeb/Repositories/rosmod/comm/src/rosmod/msg/Logger.msg" ""
)

get_filename_component(_filename "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/GetLoggers.srv" NAME_WE)
add_custom_target(_rosmod_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "rosmod" "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/GetLoggers.srv" "rosmod/Logger"
)

get_filename_component(_filename "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/SetLoggerLevel.srv" NAME_WE)
add_custom_target(_rosmod_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "rosmod" "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/SetLoggerLevel.srv" ""
)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(rosmod
  "/home/jeb/Repositories/rosmod/comm/src/rosmod/msg/Logger.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/rosmod
)

### Generating Services
_generate_srv_cpp(rosmod
  "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/Empty.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/rosmod
)
_generate_srv_cpp(rosmod
  "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/GetLoggers.srv"
  "${MSG_I_FLAGS}"
  "/home/jeb/Repositories/rosmod/comm/src/rosmod/msg/Logger.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/rosmod
)
_generate_srv_cpp(rosmod
  "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/SetLoggerLevel.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/rosmod
)

### Generating Module File
_generate_module_cpp(rosmod
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/rosmod
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(rosmod_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(rosmod_generate_messages rosmod_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/Empty.srv" NAME_WE)
add_dependencies(rosmod_generate_messages_cpp _rosmod_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jeb/Repositories/rosmod/comm/src/rosmod/msg/Logger.msg" NAME_WE)
add_dependencies(rosmod_generate_messages_cpp _rosmod_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/GetLoggers.srv" NAME_WE)
add_dependencies(rosmod_generate_messages_cpp _rosmod_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/SetLoggerLevel.srv" NAME_WE)
add_dependencies(rosmod_generate_messages_cpp _rosmod_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(rosmod_gencpp)
add_dependencies(rosmod_gencpp rosmod_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS rosmod_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(rosmod
  "/home/jeb/Repositories/rosmod/comm/src/rosmod/msg/Logger.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/rosmod
)

### Generating Services
_generate_srv_lisp(rosmod
  "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/Empty.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/rosmod
)
_generate_srv_lisp(rosmod
  "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/GetLoggers.srv"
  "${MSG_I_FLAGS}"
  "/home/jeb/Repositories/rosmod/comm/src/rosmod/msg/Logger.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/rosmod
)
_generate_srv_lisp(rosmod
  "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/SetLoggerLevel.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/rosmod
)

### Generating Module File
_generate_module_lisp(rosmod
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/rosmod
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(rosmod_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(rosmod_generate_messages rosmod_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/Empty.srv" NAME_WE)
add_dependencies(rosmod_generate_messages_lisp _rosmod_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jeb/Repositories/rosmod/comm/src/rosmod/msg/Logger.msg" NAME_WE)
add_dependencies(rosmod_generate_messages_lisp _rosmod_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/GetLoggers.srv" NAME_WE)
add_dependencies(rosmod_generate_messages_lisp _rosmod_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/SetLoggerLevel.srv" NAME_WE)
add_dependencies(rosmod_generate_messages_lisp _rosmod_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(rosmod_genlisp)
add_dependencies(rosmod_genlisp rosmod_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS rosmod_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(rosmod
  "/home/jeb/Repositories/rosmod/comm/src/rosmod/msg/Logger.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/rosmod
)

### Generating Services
_generate_srv_py(rosmod
  "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/Empty.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/rosmod
)
_generate_srv_py(rosmod
  "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/GetLoggers.srv"
  "${MSG_I_FLAGS}"
  "/home/jeb/Repositories/rosmod/comm/src/rosmod/msg/Logger.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/rosmod
)
_generate_srv_py(rosmod
  "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/SetLoggerLevel.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/rosmod
)

### Generating Module File
_generate_module_py(rosmod
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/rosmod
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(rosmod_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(rosmod_generate_messages rosmod_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/Empty.srv" NAME_WE)
add_dependencies(rosmod_generate_messages_py _rosmod_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jeb/Repositories/rosmod/comm/src/rosmod/msg/Logger.msg" NAME_WE)
add_dependencies(rosmod_generate_messages_py _rosmod_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/GetLoggers.srv" NAME_WE)
add_dependencies(rosmod_generate_messages_py _rosmod_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/jeb/Repositories/rosmod/comm/src/rosmod/srv/SetLoggerLevel.srv" NAME_WE)
add_dependencies(rosmod_generate_messages_py _rosmod_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(rosmod_genpy)
add_dependencies(rosmod_genpy rosmod_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS rosmod_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/rosmod)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/rosmod
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/rosmod)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/rosmod
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/rosmod)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/rosmod\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/rosmod
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
