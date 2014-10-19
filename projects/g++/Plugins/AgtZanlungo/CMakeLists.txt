cmake_minimum_required(VERSION 2.8)

project(ZanlungoModel)

add_definitions( -DZANLUNGO_EXPORT )

file(
	GLOB_RECURSE
	source_files
	${PLUGIN_SOURCE_DIR}/AgtZanlungo/*.cpp
	${PLUGIN_SOURCE_DIR}/AgtZanlungo/*.h
)

add_library(
	zanlungoPed
	SHARED
	${source_files}
)

target_link_libraries (zanlungoPed mengeCore ${OPENGL_LIBRARIES} ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY} )
