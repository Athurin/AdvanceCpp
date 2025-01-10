cmake_minimum_required(VERSION 3.10)

project(CircleMain)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
	CircleMain.cpp
)

#向工程添加多个特定头文件的搜索路径
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../CircleStaticLib)

#连接需要的库
target_link_libraries(${PROJECT_NAME} PRIVATE CircleStaticLib)