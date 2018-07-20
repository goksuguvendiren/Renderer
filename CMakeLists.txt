cmake_minimum_required(VERSION 3.7)
project(RayTracer)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

set(SOURCE_FILES src/main.cpp include/shapes/Shape.hpp include/Ray.hpp include/HitInfo.hpp include/shapes/Sphere.hpp src/shapes/Sphere.cpp include/shapes/Mesh.hpp include/shapes/Triangle.hpp src/shapes/Triangle.cpp src/shapes/Mesh.cpp test/hit_test.cpp include/catch2/catch.hpp)
add_executable(RayTracer ${SOURCE_FILES})