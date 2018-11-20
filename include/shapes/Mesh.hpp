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

        public:
            Mesh(const gpt::Material& m, const std::vector<gpt::shapes::Triangle>& fcs, int id) : faces(std::move(fcs)), Shape(m, fcs, id) {}
            Mesh(const Mesh& m) = delete;
            Mesh(Mesh&& m) = default;

            boost::optional<HitInfo> Hit(const Ray &ray) const;
//            boost::optional<float>   ShadowHit(const Ray& ray) const;

            const auto& Faces() const { return faces; }
        };
    }
}