cmake_minimum_required(VERSION 3.6)
project(RayTracer)

find_package(OpenCV REQUIRED)

add_library(gpt_base src/gpt_base.cpp)
target_include_directories(gpt_base PUBLIC include)
target_compile_features(gpt_base PUBLIC cxx_std_14)

set(SOURCE_FILES
    src/main.cpp
    include/shapes/Shape.hpp
    include/Ray.hpp
    include/HitInfo.hpp
    include/shapes/Sphere.hpp
    src/shapes/Sphere.cpp
    include/shapes/Mesh.hpp
    include/shapes/Triangle.hpp
    src/shapes/Triangle.cpp
    src/shapes/Mesh.cpp
    test/hit_test.cpp
    include/catch2/catch.hpp
    include/Camera.hpp
    include/ImagePlane.hpp
    src/Camera.cpp
    include/Scene.hpp
    src/Scene.cpp
    src/tinyxml/tinyxml2.cpp
    include/Image.hpp
    src/ImagePlane.cpp
    include/Transformation.hpp
    src/Transformation.cpp
    include/materials/Material.hpp
    src/materials/Material.cpp
    src/HitInfo.cpp
    include/materials/EmittingMaterial.hpp
    src/materials/EmittingMaterial.cpp
    include/lights/Light.hpp
    include/lights/PointLight.hpp
    include/Utils.hpp
    src/lights/Light.cpp
    src/lights/PointLight.cpp
    include/materials/BasicMaterial.hpp
        src/materials/BasicMaterial.cpp)
add_executable(RayTracer ${SOURCE_FILES})

add_subdirectory(src/loaders)

target_link_libraries(RayTracer PUBLIC gpt_base)
target_link_libraries(RayTracer PUBLIC ${OpenCV_LIBS})
target_link_libraries(RayTracer PUBLIC xml_loader)
