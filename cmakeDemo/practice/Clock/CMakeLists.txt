cmake_minimum_required(VERSION 3.10) 

get_filename_component(CURRENT_DIR_PATH "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE) 

get_filename_component(CURRENT_DIR_NAME "${CURRENT_DIR_PATH}" NAME) 
project(${CURRENT_DIR_NAME})

# 设置 Qt 库的地址（按需设置）右斜线 地址可能需要与系统环境变量一致
set(QT_PATH D:/Qt/qt_app/5.14.2/msvc2017_64)
# set(QT_PATH D:/Qt/qt_app/5.14.2/mingw73_64)
set(CMAKE_PREFIX_PATH ${QT_PATH}/lib/cmake)

# 指明静态库所在路径 在 MinGW 环境下，静态库文件通常是以 .a 结尾
# 在 MSVC 环境下，静态库文件通常是以 .lib 结尾
# link_directories(${QT_PATH}/lib)
# find_library(QT5_CORE_LIBRARY Qt5Core.lib PATHS ${QT_PATH}/lib)
# find_library(QT5_WIDGETS_LIBRARY Qt5Widgets.lib PATHS ${QT_PATH}/lib)
# find_library(QT5_GUI_LIBRARY Qt5Gui.lib PATHS ${QT_PATH}/lib)


# 动态库连接 一次只接受一个参数
# find_library(QT5_CORE_LIBRARY Qt5Core.dll PATHS ${QT_PATH}/bin)
# find_library(QT5_WIDGETS_LIBRARY Qt5Widgets.dll PATHS ${QT_PATH}/bin)
# find_library(QT5_GUI_LIBRARY Qt5Gui.dll PATHS ${QT_PATH}/bin)


set(CMAKE_CXX_STANDARD 11) 

if(MSVC)
	add_compile_options(/Zc:__cplusplus)
endif()

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

file(GLOB UI_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.ui")
file(GLOB RCC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*qrc")
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_BINARY_DIR}/*.h")
file(GLOB CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

qt5_wrap_ui(UI_HEADERS ${UI_FILES})

source_group("Form Files" FILES ${UI_FILES})
source_group("Resource Files" FILES ${RCC_FILES})



add_executable(${PROJECT_NAME} WIN32 ${HEADER_FILES} ${CPP_FILES} ${UI_FILES} ${RCC_FILES})


target_link_libraries(${PROJECT_NAME} 
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
)