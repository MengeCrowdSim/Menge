cmake_minimum_required(VERSION 2.8)

project(AircraftPlugin)

add_definitions( -DEXPORT )

message( STATUS "PluginSource={${PLUGIN_SOURCE_DIR}}" )
file(
	GLOB_RECURSE
	source_files
	${PLUGIN_SOURCE_DIR}/aircraft/*.cpp
	${PLUGIN_SOURCE_DIR}/aircraft/*.h
)

add_library(
	aircraft
	SHARED
	${source_files}
)

target_link_libraries (aircraft mengeCore  ${OPENGL_LIBRARIES} ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY} )

