set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../bin/)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories(../Common)

add_library(pathplanner
    pathplanner.cpp
	../Common/common.cpp
	../Common/common.h
    include/pathplanner.h)

target_include_directories(pathplanner PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE include)

# add_subdirectory(test)
