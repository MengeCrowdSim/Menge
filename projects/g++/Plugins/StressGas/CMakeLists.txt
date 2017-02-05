cmake_minimum_required(VERSION 2.8)

project(StressGasPlugin)

add_definitions( -DEXPORT )


file(
	GLOB_RECURSE
	source_files
	${PLUGIN_SOURCE_DIR}/StressGas/*.cpp
	${PLUGIN_SOURCE_DIR}/StressGas/*.h
)

add_library(
	stressGas
	SHARED
	${source_files}
)

target_link_libraries (stressGas mengeCore )

