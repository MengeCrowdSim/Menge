cmake_minimum_required(VERSION 2.8)

project(HelbingModel)

add_definitions( -DHELBING_EXPORT )

file(
	GLOB_RECURSE
	source_files
	${PLUGIN_SOURCE_DIR}/AgtHelbing/*.cpp
	${PLUGIN_SOURCE_DIR}/AgtHelbing/*.h
)

add_library(
	helbingPed
	SHARED
	${source_files}
)

target_link_libraries (helbingPed mengeCore  ${OPENGL_LIBRARIES} ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY} )
