set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets 3DCore 3DExtras 3DRender Qml 3DInput Quick QuickControls2 Network REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(../Common)
include_directories(${CMAKE_BINARY_DIR})

add_library(visualizer
	src/visualizer.cpp
	src/modelrenderer.cpp
	src/renderer.cpp
	src/scenecreator.cpp
	src/mainwindow.cpp
	src/modelmover.cpp
	src/heightmaprenderer.cpp 
	src/pathrenderer.cpp 
	src/moveable.cpp
	src/animator.cpp
	../Common/common.cpp
	../Common/common.h
	include/visualizer.h
	include/modelrenderer.h
	include/renderer.h
	include/scenecreator.h
	include/animator.h
	include/mainwindow.h
	include/modelmover.h
	include/moveable.h
	include/heightmaprenderer.h 
	include/pathrenderer.h
)

target_link_libraries(visualizer
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::3DCore
    Qt5::3DExtras
    Qt5::3DRender
    Qt5::3DInput
	Qt5::Qml
	Qt5::Quick
	Qt5::QuickControls2
)

target_include_directories(visualizer PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE include)

add_custom_command(
    TARGET visualizer
    POST_BUILD COMMAND
        ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/models"
        $<TARGET_FILE_DIR:visualizer>/models)