cmake_minimum_required(VERSION 2.8)

project(MENGE)

set(MENGE_SRC_DIR ${CMAKE_SOURCE_DIR}/../../src/Menge)
set(MENGE_EXE_DIR ${CMAKE_SOURCE_DIR}/../../Exe) 
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib/)
link_directories (${LIBRARY_OUTPUT_PATH}/)

#Where to find the header files
INCLUDE_DIRECTORIES (${MENGE_SRC_DIR}/)
INCLUDE_DIRECTORIES (${MENGE_SRC_DIR}/../include)
INCLUDE_DIRECTORIES (${MENGE_SRC_DIR}/../)

set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fpermissive" )

#find the correct OpenMP flag
FIND_PACKAGE(OpenMP)
if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OpenMP_CXX_FLAGS} -fpermissive -DNDEBUG")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${OpenMP_EXE_LINKER_FLAGS}")
endif()



ADD_SUBDIRECTORY(tinyxml)
ADD_SUBDIRECTORY(MengeCore)
ADD_SUBDIRECTORY(MengeVis)
ADD_SUBDIRECTORY(mengeMain)

file( 
  GLOB
  EXTRA_FILES
  ${CMAKE_SOURCE_DIR}/../resources/linux/*
  ${CMAKE_SOURCE_DIR}/../resources/common/*
)
file( COPY ${EXTRA_FILES} DESTINATION ${MENGE_EXE_DIR} )

file(
  GLOB
  VIS_IMAGES
  ${CMAKE_SOURCE_DIR}/../resources/common/images/*
)
file( COPY ${VIS_IMAGES} DESTINATION ${MENGE_EXE_DIR}/images )

set( CMAKE_INSTALL_ALWAYS true )
