CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

# variables declaration

SET(PROJECT gemlogd)

SET(DEPENDS_ON_PROJECTS common core mongoose)

FILE(GLOB_RECURSE HEADERS "include/*.h")

FILE(GLOB_RECURSE SOURCES "src/*.cpp")

FILE(GLOB_RECURSE MOC_SOURCES "*_automoc.cpp")

SET(CONFIG_FILES "config.ini")

# source grouping

source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES})
source_group ("Generated Files" FILES ${MOC_SOURCES})

# project definition

PROJECT(${PROJECT} CXX)

# includes

foreach(DEPENDENCY ${DEPENDS_ON_PROJECTS})
	include_directories("../${DEPENDENCY}")
endforeach()

include_directories("..")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT}")

# linking

add_executable(${PROJECT} ${HEADERS} ${SOURCES} ${MOC_SOURCES} ${CONFIG_FILES})

SET_TARGET_PROPERTIES(${PROJECT} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(
    ${PROJECT}
    ${DEPENDS_ON_PROJECTS}
    ${Boost_LIBRARIES}
    Qt5::Core
    Qt5::Sql)

# dependencies

add_dependencies(${PROJECT} ${DEPENDS_ON_PROJECTS})

# config files

# add_custom_target(copy)
foreach(ConfigFile ${CONFIG_FILES})
  add_custom_command(TARGET ${PROJECT} PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E
                         copy ${ConfigFile} $<TARGET_FILE_DIR:${PROJECT}>)
endforeach()
