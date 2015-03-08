#cmake ver req
cmake_minimum_required(VERSION 2.8.3)

if(NOT CMAKE_BUILD_TYPE )
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Select if you want to build Debug or Release" FORCE )
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/")

set(SHARED_BUILD TRUE CACHE BOOL "Set this to false if you want a static build")

set(BUILD_TESTS TRUE CACHE BOOL "build the tests")
set(BUILD_EXAMPLES TRUE CACHE BOOL "build the examples - requires GLFW")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)  #for shared libs 
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)  #for static libs 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)  #for static libs 
 
SET(CMAKE_DEBUG_POSTFIX "-d") 

if(SHARED_BUILD)
    set(SHARED_OR_STATIC SHARED)
else(SHARED_BUILD)
    set(SHARED_OR_STATIC STATIC)
endif(SHARED_BUILD)

set(project_name gimgui)
project(${project_name})

#Set source files.
set(source_files
    src/data/element.cpp
    src/data/variant.cpp
    src/gui/gui.cpp
    src/logic/allpropagator.cpp
    src/logic/attributepopulator.cpp
)

set(header_files
    include/gimgui/assert.hpp
    include/gimgui/data/element.hpp
    include/gimgui/data/element.inl
    include/gimgui/data/renderdata.hpp
    include/gimgui/data/types.hpp
    include/gimgui/data/variant.hpp
    include/gimgui/data/variant.inl
    include/gimgui/gui/gui.hpp
    include/gimgui/gui/gui.inl
    include/gimgui/logic/absolutemap.hpp
    include/gimgui/logic/absolutemap.inl
    include/gimgui/logic/allpropagator.hpp
    include/gimgui/logic/attributepopulator.hpp
    include/gimgui/logic/attributepopulator.inl
    include/gimgui/logic/event.hpp
    include/gimgui/logic/renderdatagenerator.hpp
    include/gimgui/logic/renderdatagenerator.inl
)

include_directories(include)

#C++11 required
if(NOT MSVC)
    set(CPP_11_FLAG -std=c++11)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPP_11_FLAG}")

#build
add_library(${project_name} ${SHARED_OR_STATIC} ${header_files} ${source_files})
target_link_libraries(${project_name})
    
#tests
if(BUILD_TESTS)
    include_directories(test)
    add_executable(tests "test/main.cpp"
        "test/data/elementtest.cpp"
        "test/data/varianttest.cpp"
        "test/gui/guitest.cpp"
        "test/logic/allpropagatortest.cpp"
        "test/logic/attributepopulatortest.cpp"
        "test/logic/absolutemaptest.cpp"
        "test/logic/renderdatageneratortest.cpp"
        "test/helpers/helperstest.cpp"

        "test/helpers/color.cpp"
        "test/helpers/closeenough.cpp"
        "test/helpers/vec2.cpp"
        )

    target_link_libraries(tests ${project_name})
endif(BUILD_TESTS)

#examples
if(BUILD_EXAMPLES)
    find_package(OpenGL REQUIRED)
    if(${OPENGL_FOUND})
        include_directories(${OPENGL_INCLUDE_DIRS})
    endif()
    find_package(GLFW3 REQUIRED)
    if(${GLFW_FOUND})
        include_directories(${GLFW_INCLUDE_DIRS})
    endif()

    set(common "examples/common")
    include_directories(${common})
    set(common_src 
        ${common}/gl_core_3_3.c
        ${common}/window.cpp
        ${common}/glutils/baseshader.cpp
        ${common}/glutils/buffer.cpp
        ${common}/glutils/projection.cpp
        ${common}/glutils/shader.cpp
        ${common}/glutils/uniform.cpp
        ${common}/glutils/vao.cpp
        ${common}/glutils/vertexattribute.cpp
        )

    set(simple_src "examples/simple_rendering/src")
    add_executable(simple_rendering
        ${common_src}
        ${simple_src}/main.cpp
        ${simple_src}/simplerendering.cpp
        ${simple_src}/events.cpp
        test/helpers/vec2.cpp
        test/helpers/color.cpp
        )

    target_link_libraries(simple_rendering ${project_name} ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})
endif()
	
#installation
if(WIN32)
    set(GIMGUI_MISC_DIR .)
    set(WINFIX SYSTEM)
else()
    set(GIMGUI_MISC_DIR share/gimgui)
endif()

install(
    DIRECTORY include
    DESTINATION .
    )

install(
    TARGETS ${project_name}
    RUNTIME DESTINATION bin COMPONENT bin
    LIBRARY DESTINATION lib COMPONENT bin
    ARCHIVE DESTINATION lib COMPONENT dev
    )

install(
    FILES README.md LICENSE
    DESTINATION ${GIMGUI_MISC_DIR}
    )

