//
// Created by Göksu Güvendiren on 21/07/2018.
//

#pragma once

#include <glm/vec3.hpp>
#include <string>
#include <vector>
#include <map>

#include <shapes/Shape.hpp>
#include <shapes/Triangle.hpp>
#include <shapes/Sphere.hpp>
#include <shapes/Mesh.hpp>
#include <Camera.hpp>

#include <materials/Material.hpp>
#include <iostream>
#include <lights/Light.hpp>

using MaterialFactory = std::function<std::unique_ptr<gpt::Material>(const gpt::Scene &, tinyxml2::XMLElement *)>;
extern std::map<std::string, MaterialFactory> loaders;

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
        float maxRecursionDepth;

        std::vector<gpt::Camera> cameras;
        std::vector<glm::vec3> vertices;
        std::map<std::string, glm::mat4> transformations;

        std::vector<gpt::shapes::Sphere> spheres;
        std::vector<gpt::shapes::Triangle> triangles;
        std::vector<gpt::shapes::Mesh> meshes;

        std::vector<gpt::shapes::Shape*> shapes;

        std::map<int, std::unique_ptr<gpt::Material>> materials;
        std::vector<std::unique_ptr<gpt::lights::Light>> lights;

    public:
        Scene(const glm::vec3& bg = {0, 0, 0}, const glm::vec3& al = {0, 0, 0}) : backgroundColor(bg), ambientLight(al)
        {
            shadowRayEpsilon = 1e-3;
            intersectionTestEpsilon = 1e-6;
        }
        ~Scene() = default;
        Scene(const Scene& s) = delete;
        Scene& operator=(const Scene&) = delete;

        boost::optional<HitInfo> Hit(const Ray& r) const;

        void Load(const std::string& filename);

//        void AddCamera(gpt::Camera&& cam) { cameras.push_back(std::move(cam)); }
        const gpt::Camera& GetCamera(int index) const { return cameras[index]; }
        const gpt::Material& GetMaterial(int id) const { return *(materials.find(id)->second.get()); }

        const std::vector<std::unique_ptr<gpt::lights::Light>>& Lights() const { return lights; }

        glm::vec3& GetVertex(int id) { return vertices[id - 1]; }
        glm::mat4  GetTransformation(const std::string& str) { return transformations.find(str)->second; }

        glm::vec3 BackgroundColor() const { return backgroundColor; }
        glm::vec3 AmbientColor() const { return ambientLight; }

        float ShadowRayEpsilon() const { return shadowRayEpsilon; }
    };
}
