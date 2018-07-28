cmake_minimum_required(VERSION 2.8)

project(mengeTests)
find_package(Threads REQUIRED)

set(MENGE_EXE_DIR ${CMAKE_SOURCE_DIR}/../../Exe) 
if ( ${CMAKE_BUILD_TYPE} MATCHES "Debug" )
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${MENGE_EXE_DIR}/test/debug)
else ( ${CMAKE_BUILD_TYPE} MATCHES "Debug" )
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${MENGE_EXE_DIR}/test/plugins)
ENDIF ( ${CMAKE_BUILD_TYPE} MATCHES "Debug" )
set(PLUGIN_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../../src/Plugins)
set(MENGE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../../src/Menge)
set(MENGE_LIB_DIR ${CMAKE_BINARY_DIR}/lib )
set (MENGE_ROOT_TEST_DIR ../../../../src/test/)

link_directories( ${MENGE_LIB_DIR}/)

INCLUDE_DIRECTORIES (${MENGE_SOURCE_DIR}/)
INCLUDE_DIRECTORIES (${MENGE_SOURCE_DIR}/../include)
INCLUDE_DIRECTORIES (${MENGE_SOURCE_DIR}/../)
# Enable ExternalProject CMake module
include(ExternalProject)

# Download and install GoogleTest
ExternalProject_Add(
    gtest
     URL https://github.com/google/googletest/archive/release-1.8.0.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    # Disable install step
    INSTALL_COMMAND ""
)

# Get GTest source and binary directories from CMake project
ExternalProject_Get_Property(gtest source_dir binary_dir)

# Create a libgtest target to be used as a dependency by test programs
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)

# Set libgtest properties
set_target_properties(libgtest PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/googlemock/gtest/libgtest.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

# Create a libgmock target to be used as a dependency by test programs
add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gtest)

# Set libgmock properties
set_target_properties(libgmock PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/googlemock/libgmock.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories("${source_dir}/googletest/include"
                    "${source_dir}/googlemock/include")

add_subdirectory(MengeCore)
