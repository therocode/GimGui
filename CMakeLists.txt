#cmake ver req
cmake_minimum_required(VERSION 2.8.3)

if(NOT CMAKE_BUILD_TYPE )
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Select if you want to build Debug or Release" FORCE )
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/")

set(SHARED_BUILD TRUE CACHE BOOL "Set this to false if you want a static build")

set(BUILD_TESTS TRUE CACHE BOOL "build the tests")
set(BUILD_EXAMPLES TRUE CACHE BOOL "build the examples - requires SDL2")

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

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

#Set source files.
set(source_files
    src/data/element.cpp
    src/data/codepointsize.cpp
    src/data/font.cpp
    src/data/fontloadexception.cpp
    src/data/texturecoordinates.cpp
    src/data/variant.cpp
    src/logic/allpropagator.cpp
    src/logic/attributepopulator.cpp
    src/logic/fonttexturecache.cpp
    src/util/guillotinebinpack.cpp
)

set(header_files
    include/gimgui/assert.hpp
    include/gimgui/data/bitmap.hpp
    include/gimgui/data/codepointsize.hpp
    include/gimgui/data/element.hpp
    include/gimgui/data/element.inl
    include/gimgui/data/font.hpp
    include/gimgui/data/fontloadexception.hpp
    include/gimgui/data/glyph.hpp
    include/gimgui/data/renderdata.hpp
    include/gimgui/data/rendermodes.hpp
    include/gimgui/data/rectangle.hpp
    include/gimgui/data/rectangle.inl
    include/gimgui/data/types.hpp
    include/gimgui/data/texturecoordinates.hpp
    include/gimgui/data/variant.hpp
    include/gimgui/data/variant.inl
    include/gimgui/logic/absolutemap.hpp
    include/gimgui/logic/absolutemap.inl
    include/gimgui/logic/allpropagator.hpp
    include/gimgui/logic/attributepopulator.hpp
    include/gimgui/logic/attributepopulator.inl
    include/gimgui/logic/boundarypropagator.hpp
    include/gimgui/logic/boundarypropagator.inl
    include/gimgui/logic/event.hpp
    include/gimgui/logic/fonttexturecache.hpp
    include/gimgui/logic/fonttexturecache.inl
    include/gimgui/logic/renderdatagenerator.hpp
    include/gimgui/logic/renderdatagenerator.inl
    include/gimgui/logic/rectanglepacker.hpp
    include/gimgui/util/getorfallback.hpp
    include/gimgui/util/getorfallback.inl
    include/gimgui/util/guillotinebinpack.hpp
    include/gimgui/util/resolve.hpp
    include/gimgui/util/resolve.inl
)

include_directories(include)

#C++11 required
if(NOT MSVC)
    set(CPP_11_FLAG -std=c++11)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPP_11_FLAG}")

#build
add_library(${project_name} ${SHARED_OR_STATIC} ${header_files} ${source_files})
target_link_libraries(${project_name} ${FREETYPE_LIBRARIES})
    
#tests
if(BUILD_TESTS)
    include_directories(test)
    add_executable(tests "test/main.cpp"
        "test/data/elementtest.cpp"
        "test/data/fonttest.cpp"
        "test/data/rectangletest.cpp"
        "test/data/varianttest.cpp"
        "test/logic/allpropagatortest.cpp"
        "test/logic/attributepopulatortest.cpp"
        "test/logic/absolutemaptest.cpp"
        "test/logic/boundarypropagatortest.cpp"
        "test/logic/renderdatageneratortest.cpp"
        "test/logic/rectanglepackertest.cpp"
        "test/helpers/helperstest.cpp"
        "test/util/fonttexturecachetest.cpp"
        "test/util/getorfallbacktest.cpp"
        "test/util/guillotinebinpacktest.cpp"
        "test/util/resolvetest.cpp"

        "test/helpers/color.cpp"
        "test/helpers/closeenough.cpp"
        "test/helpers/textureinterfacestub.cpp"
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
    find_package(SDL2 REQUIRED)
    if(SDL2_FOUND)
        include_directories(${SDL2_INCLUDE_DIR})
    endif()

    set(common "examples/common")
    include_directories(${common})
    set(common_src 
        ${common}/gl_core_3_3.c
        ${common}/window.cpp
        ${common}/lodepng.cpp
        ${common}/glutils/baseshader.cpp
        ${common}/glutils/buffer.cpp
        ${common}/glutils/projection.cpp
        ${common}/glutils/shader.cpp
        ${common}/glutils/texture.cpp
        ${common}/glutils/textureloader.cpp
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

    target_link_libraries(simple_rendering ${project_name} ${SDL2_LIBRARY} ${OPENGL_LIBRARIES})
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

