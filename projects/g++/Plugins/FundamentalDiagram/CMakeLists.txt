cmake_minimum_required(VERSION 2.8)

project(FDPlugin)

add_definitions( -DFDMODIFIER_EXPORT )


file(
	GLOB_RECURSE
	source_files
	${PLUGIN_SOURCE_DIR}/FundamentalDiagram/*.cpp
	${PLUGIN_SOURCE_DIR}/FundamentalDiagram/*.h
)

add_library(
	fundDiag
	SHARED
	${source_files}
)

target_link_libraries (fundDiag mengeCore  ${OPENGL_LIBRARIES} ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY} )

