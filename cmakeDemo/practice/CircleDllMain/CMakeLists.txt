cmake_minimum_required(VERSION 3.10)

project(circledllmain)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
)

# 向工程添加多个特定头文件的搜索路径  -I
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../CircleDLL)

#链接需要操作的库
target_link_libraries(${PROJECT_NAME} PRIVATE circledll)