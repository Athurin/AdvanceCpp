cmake_minimum_required(VERSION 3.10) 

get_filename_component(CURRENT_DIR_PATH "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE) 

get_filename_component(CURRENT_DIR_NAME "${CURRENT_DIR_PATH}" NAME) 
project(${CURRENT_DIR_NAME})

# 设置 Qt 库的地址（按需设置）右斜线 地址可能需要与系统环境变量一致
set(QT_PATH D:/Qt/qt_app/5.14.2/msvc2017_64)
# set(QT_PATH D:/Qt/qt_app/5.14.2/mingw73_64)
set(CMAKE_PREFIX_PATH ${QT_PATH}/lib/cmake)

#第三方DLL库文件目录
set(DLL_SOURCE_DIR D:/github_proj/advanceC++/MyThirdparty)


# 指明静态库所在路径 在 MinGW 环境下，静态库文件通常是以 .a 结尾
# 在 MSVC 环境下，静态库文件通常是以 .lib 结尾
link_directories(${QT_PATH}/lib)
# find_library(QT5_CORE_LIBRARY Qt5Core.lib PATHS ${QT_PATH}/lib)
# find_library(QT5_WIDGETS_LIBRARY Qt5Widgets.lib PATHS ${QT_PATH}/lib)
# find_library(QT5_GUI_LIBRARY Qt5Gui.lib PATHS ${QT_PATH}/lib)
# find_library(QT5_NETWORK_LIBRARY Qt5Gui.lib PATHS ${QT_PATH}/lib)

# 动态库连接 一次只接受一个参数
# find_library(QT5_CORE_LIBRARY Qt5Core.dll PATHS ${QT_PATH}/bin)
# find_library(QT5_WIDGETS_LIBRARY Qt5Widgets.dll PATHS ${QT_PATH}/bin)
# find_library(QT5_GUI_LIBRARY Qt5Gui.dll PATHS ${QT_PATH}/bin)

# 创建并设置libcrypto-1_1-x64库
# add_library(libcrypto-1_1-x64 SHARED IMPORTED)
# set_target_properties(libcrypto-1_1-x64 PROPERTIES IMPORTED_LOCATION "D:/github_proj/advanceC++/MyThirdparty/libcrypto-1_1-x64.dll")
# 创建并设置libssl-1_1-x64库
# add_library(libssl-1_1-x64 SHARED IMPORTED)
# set_target_properties(libssl-1_1-x64 PROPERTIES IMPORTED_LOCATION "D:/github_proj/advanceC++/MyThirdparty/libssl-1_1-x64.dll")

# 先获取DLL_SOURCE_DIR目录下所有.dll文件的列表，存储在DLL_FILES变量中
file(GLOB DLL_FILES "${DLL_SOURCE_DIR}/*.dll")

# 顺序错误 ；而且可执行文件名 也不是变量CURRENT_DIR_NAME
# 假设可执行文件名为${CURRENT_DIR_NAME}，.dll文件在${DLL_SOURCE_DIR}目录下，目标是复制到与可执行文件相同的目录
# add_custom_command(TARGET ${CURRENT_DIR_NAME} POST_BUILD
#                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
#                          "${DLL_SOURCE_DIR}/libcrypto-1_1-x64.dll"
#                           $<TARGET_FILE_DIR:${PROJECT_NAME}>
# )


set(CMAKE_CXX_STANDARD 11) 

if(MSVC)
	add_compile_options(/Zc:__cplusplus)
endif()

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets Gui Network REQUIRED)

# set(MAIN_CPP_FILES "main.cpp")  # 替换为实际的 main 函数所在文件名
file(GLOB UI_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.ui")
file(GLOB RCC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*qrc")
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_BINARY_DIR}/*.h")
set(MAIN_CPP_FILES "main.cpp")
file(GLOB CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(REMOVE_ITEM CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")  # 移除已经添加的 main.cpp 文件

set(CPP_FILES ${MAIN_CPP_FILES} ${CPP_FILES})


qt5_wrap_ui(UI_HEADERS ${UI_FILES})

source_group("Form Files" FILES ${UI_FILES})
source_group("Resource Files" FILES ${RCC_FILES})


# 先 生成可执行文件，再复制DLL库到可执行文件所在目录

add_executable(${PROJECT_NAME} WIN32 ${HEADER_FILES} ${CPP_FILES} ${UI_FILES} ${RCC_FILES})

# message("Target file directory: $<TARGET_FILE_DIR:${PROJECT_NAME}>")

# 为每个 DLL 文件生成复制命令
foreach(dll_file ${DLL_FILES})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${dll_file}"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMENT "Copying ${dll_file} to the target directory"
    )
endforeach()

# 链接两个库
# target_link_libraries(${PROJECT_NAME} libcrypto-1_1-x64 libssl-1_1-x64)

target_link_libraries(${PROJECT_NAME} 
	Qt5::Widgets
	Qt5::Core
	Qt5::Gui
	Qt5::Network
)