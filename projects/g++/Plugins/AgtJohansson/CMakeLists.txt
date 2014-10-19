cmake_minimum_required(VERSION 2.8)

project(JohanssonModel)

add_definitions( -DJOHANSSON_EXPORT )

file(
	GLOB_RECURSE
	source_files
	${PLUGIN_SOURCE_DIR}/AgtJohansson/*.cpp
	${PLUGIN_SOURCE_DIR}/AgtJohansson/*.h
)

add_library(
	johanssonPed
	SHARED
	${source_files}
)

target_link_libraries (johanssonPed mengeCore ${OPENGL_LIBRARIES} ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY} )
