//
// Created by Göksu Güvendiren on 20/07/2018.
//

#pragma once

#include <glm/glm.hpp>
#include <boost/optional.hpp>
#include <sstream>
#include <vector>
#include <map>
#include <memory>

#include <materials/Material.hpp>

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
    class HitInfo;

    class Shape
    {
        int id;
        const gpt::Material* material;

    protected:

    public:
        Shape() = default;
        Shape(const gpt::Material &m, int id) : id(id), material(&m)
        {}
//        Shape(const Shape& sh) = delete;

//        Shape(Shape &&sh) = default; //noexcept
//            : id(sh.id), material(sh.material)
//        {
//        }

        virtual ~Shape() = default;
        virtual boost::optional<HitInfo> Hit(const Ray &ray) const = 0;

        const gpt::Material& Material() const { return *material; }
        void Material(const gpt::Material& mat) { material = &mat; }
//
//        Shape& operator=(const Shape&) = delete;
//        Shape& operator=(Shape&&) = delete;

        int ID() const { return id; }
    };
}

