//
// Created by Göksu Güvendiren on 21/07/2018.
//

#pragma once

#include <glm/vec3.hpp>
#include <string>
#include <vector>
#include <map>
#include <memory>

//#include <shapes/Shape.hpp>
#include <shapes/Triangle.hpp>
#include <shapes/Sphere.hpp>
#include <shapes/Mesh.hpp>
#include <Camera.hpp>

#include <materials/Material.hpp>
#include <iostream>
#include <lights/Light.hpp>
#include <shapes/LightMesh.hpp>

using MaterialFactory = std::function<std::unique_ptr<gpt::Material>(const gpt::MaterialLoadContext&, tinyxml2::XMLElement *)>;
extern std::map<std::string, MaterialFactory> loaders;

namespace gpt
{
    class Shape;

    struct SceneMeta
    {
        glm::vec3 backgroundColor;
        glm::vec3 ambientLight;
        float shadowRayEpsilon          = 1e-3;
        float intersectionTestEpsilon   = 1e-6;
        float maxRecursionDepth;

        std::vector<gpt::Camera> cameras;
        std::vector<glm::vec3> vertices;
        std::map<std::string, glm::mat4> transformations;

        std::vector<gpt::shapes::Sphere> spheres;
        std::vector<gpt::Triangle> triangles;
        std::vector<gpt::shapes::Mesh> meshes;

        std::vector<gpt::Shape*> shapes;

        std::map<int, std::unique_ptr<gpt::Material>> materials;
        std::vector<std::unique_ptr<gpt::Light>> lights;
        std::vector<gpt::LightMesh> light_meshes;

//        gpt::AABB aabb;
    };

    class Camera;
    class Scene
    {
        SceneMeta meta;

    public:
        Scene(SceneMeta m) : meta(std::move(m))
        {
//            meta.aabb.Print();
        }

        ~Scene() = default;
        Scene(Scene&& s) = default;
        Scene(const Scene& s) = delete;
        Scene& operator=(const Scene&) = delete;

//        boost::optional<HitInfo> HitNaive(const Ray& r) const;
        boost::optional<HitInfo> Hit(const Ray& r) const;

//        void AddCamera(gpt::Camera&& cam) { cameras.push_back(std::move(cam)); }
        const gpt::Camera& GetCamera(int index) const { return meta.cameras[index]; }
        const gpt::Material& GetMaterial(int id) const { return *(meta.materials.find(id)->second.get()); }

        const std::vector<std::unique_ptr<gpt::Light>>& Lights() const { return meta.lights; }
        const std::vector<gpt::LightMesh>& LightMeshes() const { return meta.light_meshes; }

        glm::vec3& GetVertex(int id) { return meta.vertices[id - 1]; }
        glm::mat4  GetTransformation(const std::string& str) { return meta.transformations.find(str)->second; }

        glm::vec3 BackgroundColor() const { return meta.backgroundColor; }
        glm::vec3 AmbientColor() const { return meta.ambientLight; }

        float ShadowRayEpsilon() const { return meta.shadowRayEpsilon; }
    };
}
