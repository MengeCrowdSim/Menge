cmake_minimum_required(VERSION 2.8)

project(KaramouzasModel)

add_definitions( -DKARAMOUZAS_EXPORT )

file(
	GLOB_RECURSE
	source_files
	${PLUGIN_SOURCE_DIR}/AgtKaramouzas/*.cpp
	${PLUGIN_SOURCE_DIR}/AgtKaramouzas/*.h
)

add_library(
	karamouzasPed
	SHARED
	${source_files}
)

target_link_libraries (karamouzasPed mengeCore ${OPENGL_LIBRARIES} ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY} )

