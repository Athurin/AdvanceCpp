cmake_minimum_required(VERSION 3.10)

project(CircleStaticLib)

#生成库文件
add_library(${PROJECT_NAME} STATIC)

#添加库文件
target_sources(${PROJECT_NAME} PRIVATE
	KCircle.h
	KCircle.cpp
)