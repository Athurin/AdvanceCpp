cmake_minimum_required(VERSION 3.10)

PROJECT(allocatorTest)

set(CMAKE_CXX_STANDARD 20) #设置C++标准

add_executable(${PROJECT_NAME} 
	main.cpp
	Allocator.h
	Allocator.tcc
	Allocator.cpp )