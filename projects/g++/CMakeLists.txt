cmake_minimum_required(VERSION 2.8)

project(FULL_MENGE_PED_SIM)
if(APPLE)
    set(CMAKE_MACOSX_RPATH ON)
endif()
add_definitions(-std=c++11)
enable_testing()
# Model dlls
ADD_SUBDIRECTORY(Menge)
ADD_SUBDIRECTORY(Plugins)
ADD_SUBDIRECTORY(test)
