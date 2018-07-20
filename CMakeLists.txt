cmake_minimum_required(VERSION 3.7)
project(RayTracer)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/main.cpp)
add_executable(RayTracer ${SOURCE_FILES})