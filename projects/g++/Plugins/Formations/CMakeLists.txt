cmake_minimum_required(VERSION 2.8)

project(FormationPlugin)

add_definitions( -DFORMATIONS_EXPORT )


file(
	GLOB_RECURSE
	source_files
	${PLUGIN_SOURCE_DIR}/Formations/*.cpp
	${PLUGIN_SOURCE_DIR}/Formations/*.h
)

add_library(
	formations
	SHARED
	${source_files}
)

target_link_libraries (formations mengeCore  ${OPENGL_LIBRARIES} ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY} )

