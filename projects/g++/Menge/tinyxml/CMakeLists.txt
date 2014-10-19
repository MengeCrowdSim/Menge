cmake_minimum_required(VERSION 2.8)

project(tinyxml)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC" )

file(
	GLOB_RECURSE
	source_files
	${MENGE_SRC_DIR}/tinyxml/*.cpp
	${MENGE_SRC_DIR}/tinyxml/*.h
)


add_library(
	tinyxml
	STATIC
	${source_files}
)

#install( TARGETS tinyxml DESTINATION ${MENGE_SRC_DIR}/lib )