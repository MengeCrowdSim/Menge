cmake_minimum_required(VERSION 2.8)

project(DummyModel)

add_definitions( -DDUMMY_EXPORT )

file(
	GLOB_RECURSE
 	source_files
	${PLUGIN_SOURCE_DIR}/AgtDummy/*.cpp
	${PLUGIN_SOURCE_DIR}/AgtDummy/*.h
)

add_library(
	dummyPed
	SHARED
	${source_files}
)

target_link_libraries (dummyPed mengeCore  ${OPENGL_LIBRARIES} ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY} )

