cmake_minimum_required(VERSION 2.8)

project(TerrainPlugin)

add_definitions( -DEXPORT )


file(
	GLOB_RECURSE
	source_files
	${PLUGIN_SOURCE_DIR}/Terrain/*.cpp
	${PLUGIN_SOURCE_DIR}/Terrain/*.h
)

add_library(
	terrain
	SHARED
	${source_files}
)

target_link_libraries (terrain mengeCore mengeVis )

