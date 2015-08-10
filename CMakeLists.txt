CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

# Common settings

CMAKE_POLICY(SET CMP0003 NEW)

MACRO(add_test_ex target)
    ADD_TEST(NAME ${target} COMMAND $<TARGET_FILE:${target}> --gtest_output=xml:${CMAKE_BINARY_DIR}/build/test-reports/${target}.xml)
ENDMACRO()

SET(TEXT_TRANSFORM "C:/Program Files (x86)/Common Files/Microsoft Shared/TextTemplating/12.0/TextTransform.exe")

MACRO(add_custom_action_t4 targets)
	foreach(target ${targets})
    	add_custom_command(TARGET ${PROJECT} PRE_BUILD COMMAND ${TEXT_TRANSFORM} -I "${CMAKE_CURRENT_SOURCE_DIR}" -out "${CMAKE_CURRENT_SOURCE_DIR}/${target}" "${CMAKE_CURRENT_SOURCE_DIR}/${target}.tt")
    endforeach()
ENDMACRO()

MACRO(add_custom_action_copy targets)
	foreach(File ${targets})
	  add_custom_command(TARGET ${PROJECT} PRE_BUILD
	                     COMMAND ${CMAKE_COMMAND} -E
	                         copy ${File} $<TARGET_FILE_DIR:${PROJECT}>)
	endforeach()
ENDMACRO()

# Project settings

PROJECT(jpsutils CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)

if (CMAKE_COMPILER_IS_GNUCXX)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

# Add some multithreaded build support
MARK_AS_ADVANCED(MULTITHREADED_BUILD)
set(MULTITHREADED_BUILD 12 CACHE STRING "How many threads are used to build the project")
if(MULTITHREADED_BUILD)
    if(CMAKE_COMPILER_IS_GNUCXX)
        message(STATUS ${CMAKE_BUILD_TOOL})
        set(CMAKE_MAKE_PROGRAM "${CMAKE_MAKE_PROGRAM} -j${MULTITHREADED_BUILD}")
        message(STATUS "Added arguments to CMAKE_BUILD_TOOL: ${CMAKE_MAKE_PROGRAM}")
    elseif(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
        message(STATUS "Added parallel build arguments to CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
    endif()
endif()

# Libraries

## Qt

# (Make sure that CMAKE_PREFIX_PATH env var is set)
#set(CMAKE_AUTOMOC ON)
FIND_PACKAGE(Qt5Core)
FIND_PACKAGE(Qt5Sql)

## Boost

SET(Boost_USE_STATIC_LIBS ON)
SET(Boost_USE_MULTITHREADED ON)
FIND_PACKAGE(Boost COMPONENTS regex system thread REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
ADD_DEFINITIONS(-DBOOST_ALL_NO_LIB)

## GTest

INCLUDE(CMakeGTest.txt)

# Sub projects

add_subdirectory(Common)
add_subdirectory(Common.Tests)
add_subdirectory(Greis)
add_subdirectory(Greis.Tests)
add_subdirectory(Platform)
add_subdirectory(Platform.Tests)
add_subdirectory(DatabaseToJps)
add_subdirectory(JpsToDatabase)
add_subdirectory(jpslogd)

# Tests

ENABLE_TESTING()

ADD_TEST_EX(Common.Tests)
ADD_TEST_EX(Greis.Tests)
ADD_TEST_EX(Platform.Tests)
