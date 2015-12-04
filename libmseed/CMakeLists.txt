CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

# variables declaration

SET(PROJECT libmseed)

FILE(GLOB_RECURSE HEADERS "src/*.h")
FILE(GLOB_RECURSE SOURCES "src/*.c")

#FILE(GLOB_RECURSE SOURCES "src/testrtklib.c")
#LIST(APPEND SOURCES "src/rcv/binex.c")
#LIST(APPEND SOURCES "src/testrtklib.c")
#LIST(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/testrtklib.c")

# source grouping

source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES})

# project definition

PROJECT(${PROJECT} C)

# includes

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

# preprocessor definitions

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
# add_definitions(-D_LIB)

# linking

add_library(${PROJECT} STATIC ${HEADERS} ${SOURCES})

# SET_TARGET_PROPERTIES(${PROJECT} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(
    ${PROJECT}
    ${DEPENDS_ON_PROJECTS})
