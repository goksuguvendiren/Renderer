cmake_minimum_required(VERSION 3.7)
project(RayTracer)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

set(SOURCE_FILES src/main.cpp include/shapes/Shape.hpp include/Ray.hpp include/HitInfo.hpp include/shapes/Sphere.hpp src/shapes/Sphere.cpp include/shapes/Mesh.hpp include/shapes/Triangle.hpp src/shapes/Triangle.cpp src/shapes/Mesh.cpp test/hit_test.cpp include/catch2/catch.hpp include/Camera.hpp include/ImagePlane.hpp src/Camera.cpp include/Scene.hpp src/Scene.cpp src/tinyxml/tinyxml2.cpp include/Image.hpp src/ImagePlane.cpp include/Transformation.hpp src/Transformation.cpp)
add_executable(RayTracer ${SOURCE_FILES})

target_link_libraries(RayTracer ${OpenCV_LIBS})
