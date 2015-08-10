CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

# variables declaration

SET(PROJECT Greis)

SET(DEPENDS_ON_PROJECTS Common)

FILE(GLOB_RECURSE HEADERS "${PROJECT}/*.h")
FILE(GLOB ROOT_HEADERS "${PROJECT}/*.h")
FILE(GLOB STDMESSAGE_HEADERS "${PROJECT}/StdMessage/*.h")
FILE(GLOB CUSTOMTYPE_HEADERS "${PROJECT}/CustomType/*.h")

FILE(GLOB_RECURSE SOURCES "src/*.cpp")
FILE(GLOB ROOT_SOURCES "src/*.cpp")
FILE(GLOB STDMESSAGE_SOURCES "src/StdMessage/*.cpp")
FILE(GLOB CUSTOMTYPE_SOURCES "src/CustomType/*.cpp")

FILE(GLOB_RECURSE MOC_SOURCES "*_automoc.cpp")

# source grouping

source_group ("Header Files" FILES ${ROOT_HEADERS})
source_group ("Header Files\\StdMessage" FILES ${STDMESSAGE_HEADERS})
source_group ("Header Files\\CustomType" FILES ${CUSTOMTYPE_HEADERS})

source_group ("Source Files" FILES ${ROOT_SOURCES})
source_group ("Source Files\\StdMessage" FILES ${STDMESSAGE_SOURCES})
source_group ("Source Files\\CustomType" FILES ${CUSTOMTYPE_SOURCES})

source_group ("Generated Files" FILES ${MOC_SOURCES})

# project definition

PROJECT(${PROJECT} CXX)

# includes

foreach(DEPENDENCY ${DEPENDS_ON_PROJECTS})
	include_directories("../${DEPENDENCY}")
endforeach()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT}/StdMessage")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT}/CustomType")

# linking

add_library(${PROJECT} STATIC ${HEADERS} ${SOURCES} ${MOC_SOURCES})

SET_TARGET_PROPERTIES(${PROJECT} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(
    ${PROJECT}
    ${DEPENDS_ON_PROJECTS}
    ${Boost_LIBRARIES}
    Qt5::Core
	Qt5::Sql)
