//
// Created by Göksu Güvendiren on 20/07/2018.
//

#pragma once

#include <vector>
#include <map>
#include "shapes/Shape.hpp"
#include "lights/Light.hpp"
#include "Triangle.hpp"

namespace gpt
{
    namespace shapes
    {
        class Mesh : public Shape
        {
        protected:
            std::vector<gpt::Triangle> faces;
            gpt::AABB aabb;

        public:
            Mesh(const gpt::Material& m, std::vector<gpt::Triangle>&& fcs, int id) : Shape(m, id), faces(std::move(fcs))
            {
                std::vector<Triangle*> tris;
                tris.reserve(faces.size());
                for (auto& t : faces)
                {
                    tris.push_back(&t);
                }
                aabb = gpt::AABB(tris);
            }

            Mesh(const Mesh& m) = delete;
            Mesh(Mesh&& rhs) = default;// noexcept : Shape(std::move(rhs)), faces(std::move(rhs.faces)) {}

            boost::optional<HitInfo> Hit(const Ray &ray) const;

            glm::vec3 Min()     const { return aabb.Min(); }
            glm::vec3 Max()     const { return aabb.Max(); }
            glm::vec3 Middle()  const { return aabb.Middle(); }

            bool Emitting() const { return material->Emitting(); }
            const auto& Faces() const { return faces; }
        };
    }
}