CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

# variables declaration

SET(PROJECT core)

SET(DEPENDS_ON_PROJECTS common)

FILE(GLOB_RECURSE HEADERS "${PROJECT}/*.h")
FILE(GLOB ROOT_HEADERS "${PROJECT}/*.h")

FILE(GLOB_RECURSE SOURCES "src/*.cpp")
FILE(GLOB ROOT_SOURCES "src/*.cpp")

FILE(GLOB_RECURSE MOC_SOURCES "*_automoc.cpp")

# source grouping

source_group ("Header Files" FILES ${ROOT_HEADERS})
source_group ("Source Files" FILES ${ROOT_SOURCES})
source_group ("Generated Files" FILES ${MOC_SOURCES})

# project definition

PROJECT(${PROJECT} CXX)

# includes

foreach(DEPENDENCY ${DEPENDS_ON_PROJECTS})
	include_directories("../${DEPENDENCY}")
endforeach()

include_directories("../libmseed/src")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT}")

# preprocessor definitions

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# linking

add_library(${PROJECT} STATIC ${HEADERS} ${SOURCES} ${MOC_SOURCES})

SET_TARGET_PROPERTIES(${PROJECT} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(
    ${PROJECT}
    ${DEPENDS_ON_PROJECTS}
    ${Boost_LIBRARIES}
    Qt5::Core
	Qt5::Sql)
