cmake_minimum_required(VERSION 2.8)

project(MengeExe)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib/)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MENGE_EXE_DIR})
INCLUDE_DIRECTORIES (${MENGE_SRC_DIR}/../thirdParty/)

file(
	GLOB_RECURSE
	source_files
	${MENGE_SRC_DIR}/mengeMain/*.cpp  
	${MENGE_SRC_DIR}/mengeMain/*.h
)

add_executable(
	menge
	${source_files}
)

target_link_libraries (menge
  mengeCore
  mengeVis
  ${SDL2_LIBRARIES}
  )

#file( COPY ${CMAKE_SOURCE_DIR}/../resources/linux DESTINATION ${MENGE_EXE_DIR} )
#file( COPY ${CMAKE_SOURCE_DIR}/../resources/common DESTINATION ${MENGE_EXE_DIR} )
#install( TARGETS menge DESTINATION ${MENGE_EXE_DIR} )
