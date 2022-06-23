cmake_minimum_required(VERSION 3.16)
project(LibJR CXX)

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type was specified, defaulting to '${default_build_type}'")
    set(CMAKE_BUILD_TYPE "${default_build_type}")
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

find_program(CCACHE ccache)
if(CCACHE)
    message(STATUS "Found ccache, using it")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
else()
    message(WARNING "Did not find ccache, consider installing it for faster compiles")
endif()

set(CMAKE_CXX_STANDARD 20)
add_compile_options("-Wno-c99-designator")
add_compile_options("-fdebug-prefix-map=${PROJECT_SOURCE_DIR}=.")
add_compile_options("-ffile-prefix-map=${PROJECT_SOURCE_DIR}=.")

include_directories(.)

add_library(LibJR
    JR/StringView.cpp
    JR/String.cpp
    )
set_property(TARGET LibJR PROPERTY POSITION_INDEPENDENT_CODE ON)

add_library(LibCore
    Core/Folder.cpp
    Core/MappedFile.cpp
    Core/System.cpp
    Core/Threads.cpp
    )
set_property(TARGET LibCore PROPERTY POSITION_INDEPENDENT_CODE ON)

add_library(LibCli
    Cli/ArgumentParser.cpp
    Cli/ProgressBar.cpp
    )
set_property(TARGET LibCli PROPERTY POSITION_INDEPENDENT_CODE ON)

option(BUILD_TESTS "build tests" OFF)
if(${BUILD_TESTS})
    add_subdirectory(Tests)
endif()
