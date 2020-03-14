cmake_minimum_required(VERSION 3.8)
project(Bachelor)

# cmake initial setup
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH $ENV{QT_DIR})
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -Od") # to disable optimization in RelWithDebInfo by default, for faster debugging

# qt setup
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt5QuickCompiler)
qtquick_compiler_add_resources(RESOURCES visualizer/resources.qrc)

# project setup
add_subdirectory(pathplanner)
add_subdirectory(visualizer)
include_directories(${CMAKE_BINARY_DIR})
include_directories(common)

add_executable(Bachelor WIN32 
	main.cpp
	common/common.cpp
	common/common.h
	${RESOURCES}
)

target_link_libraries(Bachelor pathplanner visualizer
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

add_custom_command(
    TARGET Bachelor
    POST_BUILD COMMAND
        ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        $<TARGET_FILE_DIR:Bachelor>/assets)