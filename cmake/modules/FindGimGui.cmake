set( GIMGUI_FOUND false)

find_path(
    GIMGUI_INCLUDE_DIR
    gimgui/assert.hpp
    PATH_SUFFIXES
    include
    PATHS
    ${GIMGUI_ROOT}
    ${GIMGUIDIR}
    $ENV{GIMGUIDIR}
    $ENV{GIMGUI_ROOT}
    /usr
    /usr/local
    ~/Library/Frameworks
    /Library/Frameworks
    /sw
    /opt/local
    /opt/csw
    /opt
    "C:/Program Files (x86)"
    "C:/Program Files (x86)/gimgui"
)

set(FIND_GimGui_LIB_PATHS
    ${GIMGUI_ROOT}
    ${GIMGUIDIR}
    $ENV{GIMGUIDIR}
    $ENV{GIMGUI_ROOT}
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt
    "C:/Program Files (x86)"
    "C:/Program Files (x86)/gimgui"
)

#list(FIND GimGui_FIND_COMPONENTS "rendering" GimGui_NEEDS_GLM)

#if(GimGui_NEEDS_GLM EQUAL -1)
#    message("i don't need glm!")
#else()
#    message("i need glm!")
#endif()

set(GIMGUI_FOUND)
set(GIMGUI_LIBRARY)
set(GIMGUI_INCLUDE_DIR)

set(GIM_PRESERVE_LIBS false CACHE "" INTERNAL)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(GIM_LIBRARY_SUFFIX "-d")
    set(GIM_LIBRARY_ANTISUFFIX "")
    set(BUILD_TYPE "debug")
    set(ANTI_BUILD_TYPE "release")
else()
    set(GIM_LIBRARY_SUFFIX "")
    set(GIM_LIBRARY_ANTISUFFIX "-d")
    set(BUILD_TYPE "release")
    set(ANTI_BUILD_TYPE "debug")
endif()

if(NOT (${GIM_LAST_MODE} MATCHES ${BUILD_TYPE}))
    unset(GIM_LAST_MODE CACHE)
    set(GIM_LAST_MODE ${BUILD_TYPE} CACHE "" INTERNAL)
    set(GIM_RESET_LIBS true)
else()
    set(GIM_RESET_LIBS false)
endif()

set(FIND_GimGui_NAME gimgui)

if(${GIM_RESET_LIBS})
    unset(GimGui_LIBRARY CACHE)
endif()

find_library(GimGui_LIBRARY
    NAMES ${FIND_GimGui_NAME}
    PATH_SUFFIXES lib64 lib
    PATHS ${FIND_GimGui_LIB_PATHS})

#second pass with anti suffix
if(NOT GimGui_LIBRARY)
    #let the user know
    if(NOT GimGui_FIND_QUIETLY)
        message(WARNING "Failed to find ${BUILD_TYPE} version of GimGui. Will try to find ${ANTI_BUILD_TYPE} version instead.")
    endif()

    if(${GIM_RESET_LIBS})
        unset(GimGui_LIBRARY CACHE)
    endif()

    find_library(GimGui_LIBRARY
        NAMES ${FIND_GimGui_COMPONENT_NAME}${GIM_LIBRARY_ANTISUFFIX}
        PATH_SUFFIXES lib64 lib
        PATHS ${FIND_GimGui_LIB_PATHS})
endif()

if(NOT GimGui_LIBRARY)
    set(FIND_GimGui_MISSING "${FIND_GimGui_MISSING} GimGui_LIBRARY")
else()
    set(GIMGUI_LIBRARY ${GIMGUI_LIBRARY} ${GimGui_LIBRARY})
endif()

find_library(
    GIMGUI_LIBRARY
    gim-ui
    PATH_SUFFIXES
    lib
    lib64
    PATHS
    /usr
    /usr/local
    ${GIMGUI_ROOT}
    ${GIMGUIDIR}
    $ENV{GIMGUIDIR}
    $ENV{GIMGUI_ROOT}
)

if(NOT GIMGUI_INCLUDE_DIR OR FIND_GimGui_MISSING)
    set(GIMGUI_FOUND)
    set(GIMGUI_INCLUDE_DIRS)
    set(GIMGUI_LIBRARIES)
    if(GimGui_FIND_REQUIRED)
        message(FATAL_ERROR "GimGui not found.")
    elseif(NOT GimGui_FIND_QUIETLY)
        message(STATUS "GimGui not found.")
    endif()
else()
    set(GIMGUI_FOUND true)
    if(NOT GimGui_FIND_QUIETLY)
        message(STATUS "GimGui found: ${GIMGUI_INCLUDE_DIR}")
    endif()
    set(GIMGUI_INCLUDE_DIRS ${GIMGUI_INCLUDE_DIR})
    set(GIMGUI_LIBRARIES ${GIMGUI_LIBRARY})
endif()
