CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

# variables declaration

SET(PROJECT common)

FILE(GLOB_RECURSE HEADERS "*.h")

FILE(GLOB_RECURSE SOURCES "*.cpp")

FILE(GLOB_RECURSE MOC_SOURCES "*_automoc.cpp")

# source grouping

source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES})
source_group ("Generated Files" FILES ${MOC_SOURCES})

# project definition

PROJECT(${PROJECT} CXX)

# includes

include_directories("..")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT}")

# linking

add_library(${PROJECT} STATIC ${HEADERS} ${SOURCES} ${MOC_SOURCES})

SET_TARGET_PROPERTIES(${PROJECT} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(
    ${PROJECT}
    ${DEPENDS_ON_PROJECTS}
#   ${Boost_LIBRARIES}
    Qt5::Core
    debug ${GTEST_DEBUG_LIBS}
    optimized ${GTEST_RELEASE_LIBS}
    ${Pthread})
