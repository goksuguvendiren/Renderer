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
#include <AABB.hpp>

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

    protected:
        gpt::AABB aabb;

    public:
        Shape(const gpt::Material& m, const glm::vec3& mins, const glm::vec3& maxs, int id) : material(&m), id(id), aabb(this, mins, maxs) {}
        Shape(const gpt::Material& m, const std::vector<gpt::shapes::Triangle>& faces, int id);
        virtual ~Shape() = default;
        virtual boost::optional<HitInfo> Hit(const Ray &ray) const = 0;

        const gpt::Material& Material() const { return *material; }

        glm::vec3 Min()     const { return aabb.Min(); }
        glm::vec3 Max()     const { return aabb.Max(); }
        glm::vec3 Middle()  const { return aabb.Middle(); }

        int ID() const { return id; }
    };
}

