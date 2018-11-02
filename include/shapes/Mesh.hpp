//
// Created by Göksu Güvendiren on 20/07/2018.
//

#pragma once

#include <vector>
#include <map>
#include "shapes/Shape.hpp"
#include "Triangle.hpp"

namespace gpt
{
    namespace shapes
    {
        class Mesh : public Shape
        {
            std::vector<gpt::shapes::Triangle> faces;
            std::multimap<int, int> vertex_triangle_associtations;

        public:

            Mesh(const gpt::Material& m) : Shape(m) {}
            Mesh(const Mesh& m) = delete;
            Mesh(Mesh&& m) = default;

            void AddFace(Triangle&& face)
            {
                faces.push_back(std::move(face));
            }

            boost::optional<HitInfo> Hit(const Ray &ray) const;
//            boost::optional<float>   ShadowHit(const Ray& ray) const;

            const auto& Faces() const { return faces; }

            static std::vector<gpt::shapes::Mesh> Load(gpt::Scene& scene, tinyxml2::XMLElement *elem);
        };
    }
}