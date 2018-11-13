//
// Created by Göksu Güvendiren on 20/07/2018.
//

#pragma once

#include <glm/glm.hpp>
#include <boost/optional.hpp>
#include "HitInfo.hpp"
#include <sstream>
#include <vector>
#include <map>
#include <memory>

namespace gpt
{
    struct ShapeLoadContext
    {
        const std::vector<glm::vec3>& vertices;
        glm::vec3 GetVertex(int id) const { return vertices[id - 1]; }

        const std::map<std::string, glm::mat4>& transformations;
        glm::mat4 GetTransformation(const std::string& str) const { return transformations.find(str)->second; }

        const std::map<int, std::unique_ptr<gpt::Material>>& materials;
        const gpt::Material& GetMaterial(int id) const { return *(materials.find(id)->second.get()); }

        ShapeLoadContext(const std::vector<glm::vec3>& verts,
                         const std::map<std::string, glm::mat4>& transforms,
                         const std::map<int, std::unique_ptr<gpt::Material>>& mats) : vertices(verts), transformations(transforms), materials(mats)
        {}
    };

    class Ray;
    class Material;

    class Shape
    {
        int id;
        const gpt::Material* material;

    public:
        explicit Shape(const gpt::Material& m, int id) : material(&m), id(id) {}
        virtual ~Shape() = default;
        virtual boost::optional<HitInfo> Hit(const Ray &ray) const = 0;
//            virtual boost::optional<float> ShadowHit(const Ray &ray) const = 0;

        const gpt::Material& Material() const { return *material; }
//            virtual bool isArtificial() const = 0;

//            virtual glm::vec3 Min() const = 0;
//            virtual glm::vec3 Max() const = 0;
//            virtual glm::vec3 Middle() const = 0;

        int ID() const { return id; }
    };
}

