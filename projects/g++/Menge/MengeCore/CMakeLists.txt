cmake_minimum_required(VERSION 2.8)

project(MENGE_CORE)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${MENGE_EXE_DIR})

INCLUDE_DIRECTORIES (${MENGE_SRC_DIR}/MengeCore/resources)

file(
	GLOB_RECURSE
	source_files
	${MENGE_SRC_DIR}/MengeCore/*.cpp
	${MENGE_SRC_DIR}/MengeCore/*.h
)


add_library(
	mengeCore
	SHARED
	${database_files}
	${source_files}
)

target_link_libraries ( mengeCore dl )

install( TARGETS mengeCore DESTINATION ${LIBRARY_OUTPUT_PATH} )