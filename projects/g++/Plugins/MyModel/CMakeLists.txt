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
        ../../../../src/Plugins/MyPedestrianModel/Cell.cpp ../../../../src/Plugins/MyPedestrianModel/Cell.h ../../../../src/Plugins/MyPedestrianModel/Grid.cpp ../../../../src/Plugins/MyPedestrianModel/Grid.h ../../../../src/Plugins/MyPedestrianModel/DensityField.cpp ../../../../src/Plugins/MyPedestrianModel/DensityField.h)

target_link_libraries (myModel mengeCore )

