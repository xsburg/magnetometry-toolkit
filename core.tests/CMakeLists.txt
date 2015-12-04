CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

# variables declaration

SET(PROJECT core.tests)

SET(DEPENDS_ON_PROJECTS common core)

FILE(GLOB_RECURSE HEADERS "*.h")

FILE(GLOB_RECURSE SOURCES "*.cpp")

FILE(GLOB_RECURSE MOC_SOURCES "*_automoc.cpp")

SET(CONFIG_FILES "config.test.ini")

SET(TEXT_TEMPLATES "tests.h")

# check 64 bit
#if(CMAKE_SIZEOF_VOID_P EQUALS 4)
#    set(HAVE_64_BIT 0)
#else()
#    set(HAVE_64_BIT 1)
#endif()

if (MSVC)
	# Now we support only x86 build with this. Be aware that x64 configuration will fail with a strange error 'Driver not found'
	SET (DEPLOY_FILES ${DEPLOY_FILES} "C:/MySQL/MySQLConnectorC6.1/lib/libmysql.dll")
endif()

# source grouping

source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES})
source_group ("Generated Files" FILES ${MOC_SOURCES})

# project definition

PROJECT(${PROJECT} CXX)

# includes

include_directories("..")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

# linking

add_executable(${PROJECT} ${HEADERS} ${SOURCES} ${MOC_SOURCES} ${CONFIG_FILES} ${TEXT_TEMPLATES})

SET_TARGET_PROPERTIES(${PROJECT} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(
    ${PROJECT}
    ${DEPENDS_ON_PROJECTS}
#   ${Boost_LIBRARIES}
    Qt5::Core
    Qt5::Sql
    Qt5::Network
    debug ${GTEST_DEBUG_LIBS}
    optimized ${GTEST_RELEASE_LIBS}
    ${Pthread})

# dependencies

add_dependencies(${PROJECT} googletest ${DEPENDS_ON_PROJECTS})

# add_custom_target(copy)
add_custom_action_copy(${CONFIG_FILES})
if (DEPLOY_FILES)
    add_custom_action_copy(${DEPLOY_FILES})
endif()
if (MSVC)
	add_custom_action_t4(${TEXT_TEMPLATES})
endif()
