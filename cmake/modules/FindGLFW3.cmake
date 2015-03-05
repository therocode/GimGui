# Locate the glfw library
# This module defines the following variables:
# GLFW_LIBRARY, the name of the library;
# GLFW_INCLUDE_DIR, where to find glfw include files.
# GLFW_FOUND, true if both the GLFW_LIBRARY and GLFW_INCLUDE_DIR have been found.
#
# To help locate the library and include file, you could define an environment variable called
# GLFW_ROOT which points to the root of the glfw library installation. This is pretty useful
# on a Windows platform.
#
#
# Usage example to compile an "executable" target to the glfw library:
#
# FIND_PACKAGE (glfw REQUIRED)
# INCLUDE_DIRECTORIES (${GLFW_INCLUDE_DIR})
# ADD_EXECUTABLE (executable ${EXECUTABLE_SRCS})
# TARGET_LINK_LIBRARIES (executable ${GLFW_LIBRARY})
#
# TODO:
# Allow the user to select to link to a shared library or to a static library.

#Search for the include file...
FIND_PATH(GLFW_INCLUDE_DIRS GLFW/glfw3.h
  $ENV{PROGRAMFILES}/GLFW/include
  /usr/include/
  /usr/local/include/
  # By default headers are under GLFW subfolder
  /usr/include/GLFW
  /usr/local/include/GLFW
  ${GLFW_ROOT}/include/
)

FIND_LIBRARY(GLFW_LIBRARIES
  NAMES libglfw3.a glfw3 GLFW3.lib glfw
  PATHS
  /usr/lib64
  /usr/lib
  /usr/local/lib64
  /usr/local/lib
  /sw/lib
  /opt/local/lib
  ${GLFW_ROOT}/lib-msvc100/release
  ${GLFW_ROOT}/lib
  $ENV{PROGRAMFILES}/GLFW/lib
)
IF( APPLE )
    find_library(IOKIT NAMES IOKit)
    #find_library(APPKIT NAMES AppKit)
    find_library(COREVIDEO NAMES CoreVideo)
    find_library(COCOA NAMES Cocoa)
    SET(GLFW_LIBRARIES ${GLFW_LIBRARIES} ${IOKIT} ${COREVIDEO} ${COCOA})
endif( APPLE )

IF(GLFW_LIBRARIES AND GLFW_INCLUDE_DIRS)
  SET(GLFW_FOUND TRUE)
  message(STATUS "Found GLFW3: ${GLFW_LIBRARIES}")
ELSE()
  message(STATUS "GLFW3 NOT found!")
ENDIF(GLFW_LIBRARIES AND GLFW_INCLUDE_DIRS)

#if(GLFW_FOUND)
#  MARK_AS_ADVANCED(GLFW_INCLUDE_DIRS GLFW_LIBRARIES)
#endif(GLFW_FOUND)