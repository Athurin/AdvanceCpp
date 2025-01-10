cmake_minimum_required(VERSION 3.10)

project(circledll)

#生成库文件
add_library(${PROJECT_NAME} SHARED)

#添加源文件
target_sources(${PROJECT_NAME} PRIVATE
    circle.h
    circle.cpp
)

#针对特定的目标,定义宏
target_compile_definitions(${PROJECT_NAME} PRIVATE CIRCLE_DLL)


# 设置库的输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "Circle"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)