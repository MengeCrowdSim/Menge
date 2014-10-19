cmake_minimum_required(VERSION 2.8)

project(SamplePlugin)

add_definitions( -DEXPORT )


file(
	GLOB_RECURSE
	source_files
	${PLUGIN_SOURCE_DIR}/SampleProjectTemplate/*.cpp
	${PLUGIN_SOURCE_DIR}/SampleProjectTemplate/*.h
)

add_library(
	fundDiag
	SHARED
	${source_files}
)

target_link_libraries (sample mengeCore  ${OPENGL_LIBRARIES} ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY} )

