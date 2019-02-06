cmake_minimum_required(VERSION 2.8)

project(MyModelModel)

add_definitions( -DEXPORT )

file(
	GLOB_RECURSE
 	source_files
	${PLUGIN_SOURCE_DIR}/MyPedestrianModel/*.cpp
	${PLUGIN_SOURCE_DIR}/MyPedestrianModel/*.h
)

add_library(
	myModel
	SHARED
	${source_files}
)

target_link_libraries (myModel mengeCore )

