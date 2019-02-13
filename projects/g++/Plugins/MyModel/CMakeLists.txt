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
        ../../../../src/Plugins/MyPedestrianModel/Cell.cpp ../../../../src/Plugins/MyPedestrianModel/Cell.h ../../../../src/Plugins/MyPedestrianModel/Grid.cpp ../../../../src/Plugins/MyPedestrianModel/Grid.h ../../../../src/Plugins/MyPedestrianModel/DensityField.cpp ../../../../src/Plugins/MyPedestrianModel/DensityField.h ../../../../src/Plugins/MyPedestrianModel/CellFace.cpp ../../../../src/Plugins/MyPedestrianModel/CellFace.h ../../../../src/Plugins/MyPedestrianModel/PopulationManager.cpp ../../../../src/Plugins/MyPedestrianModel/PopulationManager.h ../../../../src/Plugins/MyPedestrianModel/Person.cpp ../../../../src/Plugins/MyPedestrianModel/Person.h ../../../../src/Plugins/MyPedestrianModel/Group.cpp ../../../../src/Plugins/MyPedestrianModel/Group.h)

target_link_libraries (myModel mengeCore )

