# Copyright: (C) 2012 Robotics Brain and Cognitive Sciences, Istituto Italiano di Tecnologia
# Authors: Elena Ceseracciu
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

SET(PROJECTNAME eventRepeaterIDL)
PROJECT(${PROJECTNAME})

set(THRIFT_HEADERS)
set(THRIFT_SOURCES)
set(fileList eventRepeater_thrift.cmake)
foreach(scriptFile ${fileList})
  include(${scriptFile})
  list(APPEND THRIFT_HEADERS ${headers})
  list(APPEND THRIFT_SOURCES ${sources})
endforeach()

set(generated_libs_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE INTERNAL "Generated-files libraries include directory")
include_directories(${generated_libs_include_dir} ${YARP_INCLUDE_DIRS})

add_library(${PROJECTNAME} ${THRIFT_HEADERS} ${THRIFT_SOURCES})
set_target_properties(${PROJECTNAME} PROPERTIES LINKER_LANGUAGE CXX)


