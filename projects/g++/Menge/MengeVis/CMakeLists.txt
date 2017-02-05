cmake_minimum_required(VERSION 2.8)

project(MENGE_VIS)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${MENGE_EXE_DIR})

# Move this stuff to MengeVis
FIND_PACKAGE(SDL)
if(NOT SDL_FOUND)
	message(STATUS "I will try to link against local libraries")
	INCLUDE_DIRECTORIES (${MENGE_SRC_DIR}/include)
	LINK_DIRECTORIES  (${MENGE_SRC_DIR}/lib)
	SET(SDL_LIBRARY "SDL")
	SET(SDLMAIN_LIBRARY "SDLmain")
else()
	INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})
endif()

FIND_PACKAGE(SDL_ttf)
if(NOT SDLTTF_FOUND)
	MESSAGE(STATUS "I will try to link against local libraries")
	INCLUDE_DIRECTORIES (${MENGE_SRC_DIR}/include)
	LINK_DIRECTORIES  (${MENGE_SRC_DIR}/lib)
	SET(SDLTTF_LIBRARY "SDL_ttf")
else()
	INCLUDE_DIRECTORIES(${SDLTTF_INCLUDE_DIR})
endif()

FIND_PACKAGE(SDL_image)
if(NOT SDLIMAGE_FOUND)
	message(STATUS "I will try to link against local libraries")
	INCLUDE_DIRECTORIES (${MENGE_SRC_DIR}/include)
	LINK_DIRECTORIES  (${MENGE_SRC_DIR}/lib)
	SET(SDLIMAGE_LIBRARY SDL_image)
else()
	INCLUDE_DIRECTORIES(${SDLIMAGE_INCLUDE_DIR})
endif()

FIND_PACKAGE(OpenGL REQUIRED)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIRS})

FIND_PACKAGE(PNG REQUIRED)

file(
	GLOB_RECURSE
	source_files
	${MENGE_SRC_DIR}/MengeVis/*.cpp
	${MENGE_SRC_DIR}/MengeVis/*.h
)


add_library(
	mengeVis
	SHARED
	${database_files}
	${source_files}
)

target_link_libraries ( mengeVis  dl tinyxml ${OPENGL_LIBRARIES} ${SDL_LIBRARY} ${PNG_LIBRARY}  ${SDLIMAGE_LIBRARY}  ${SDLTTF_LIBRARY} )

install( TARGETS mengeCore DESTINATION ${LIBRARY_OUTPUT_PATH} )