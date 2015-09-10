CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

# variables declaration

SET(PROJECT mongoose)

FILE(GLOB_RECURSE HEADERS "mongoose.h")
FILE(GLOB_RECURSE SOURCES "mongoose.c")

# source grouping

source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES})

# project definition

PROJECT(${PROJECT} C)

# includes

foreach(DEPENDENCY ${DEPENDS_ON_PROJECTS})
	include_directories("../${DEPENDENCY}")
endforeach()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

# preprocessor definitions

add_definitions(-DMONGOOSE_ENABLE_THREADS)

# linking

add_library(${PROJECT} STATIC ${HEADERS} ${SOURCES})

# SET_TARGET_PROPERTIES(${PROJECT} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(
    ${PROJECT}
    ${DEPENDS_ON_PROJECTS})
