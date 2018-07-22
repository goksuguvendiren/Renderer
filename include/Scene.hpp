//
// Created by Göksu Güvendiren on 21/07/2018.
//

#ifndef RAYTRACER_SCENE_HPP
#define RAYTRACER_SCENE_HPP

#include <glm/vec3.hpp>
#include <string>
#include <shapes/Shape.hpp>
#include <vector>
#include <shapes/Triangle.hpp>
#include <shapes/Sphere.hpp>
#include <map>
#include "Camera.hpp"

namespace gpt
{
    class Camera;
    class Scene
    {
        std::string path;

        glm::vec3 backgroundColor;
        glm::vec3 ambientLight;
        float shadowRayEpsilon;
        float intersectionTestEpsilon;

        std::vector<gpt::Camera> cameras;
        std::vector<glm::vec3> vertices;
        std::map<std::string, glm::mat4> transformations;

        std::vector<gpt::shapes::Sphere> spheres;
        std::vector<gpt::shapes::Triangle> triangles;
//        std::vector<gpt::shapes::Mesh> meshes;

        std::vector<std::unique_ptr<gpt::shapes::Shape>> shapes;

    public:
        Scene(glm::vec3 bg = {0, 0, 0}, glm::vec3 al = {0, 0, 0})
        {
            shadowRayEpsilon = 1e-3;
            intersectionTestEpsilon = 1e-6;
        }
        ~Scene() = default;
        Scene(const Scene& s) = delete;
        Scene& operator=(const Scene&) = delete;

        boost::optional<HitInfo> Hit(const Ray& r) const;

        void AddShape(std::unique_ptr<gpt::shapes::Shape>&& shape) { shapes.push_back(std::move(shape)); }
        void Load(const std::string& filename);

//        void AddCamera(gpt::Camera&& cam) { cameras.push_back(std::move(cam)); }
        const gpt::Camera& GetCamera(int index) const { return cameras[index]; }

        glm::vec3& GetVertex(int id) { return vertices[id - 1]; }
        glm::mat4  GetTransformation(const std::string& str) { return transformations.find(str)->second; }
    };
}

#endif //RAYTRACER_SCENE_HPP
