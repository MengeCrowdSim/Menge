cmake_minimum_required(VERSION 2.8)

project(GcfModel)

add_definitions( -DGCF_EXPORT )


file(
	GLOB_RECURSE
	source_files
	${PLUGIN_SOURCE_DIR}/AgtGCF/*.cpp
	${PLUGIN_SOURCE_DIR}/AgtGCF/*.h
)

add_library(
	gcfPed
	SHARED
	${source_files}
)

target_link_libraries ( gcfPed mengeCore mengeVis )

