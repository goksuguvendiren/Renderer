//
// Created by Göksu Güvendiren on 20/07/2018.
//

#ifndef RAYTRACER_MESH_HPP
#define RAYTRACER_MESH_HPP

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
            int id;
//            ShadingMode shmode;

            std::vector<gpt::shapes::Triangle> faces;
            std::multimap<int, int> vertex_triangle_associtations;

        public:

            Mesh(int mid = 1) : id(mid) {}
            Mesh(const Mesh& m) = delete;
            Mesh(Mesh&& m) = default;

            void AddFace(Triangle&& face)
            {
                faces.push_back(std::move(face));
            }

//            void InsertVT(Triangle face);

            boost::optional<HitInfo> Hit(const Ray &ray) const;
            boost::optional<float>   ShadowHit(const Ray& ray) const;

            int ID() const { return id; }
            const auto& Faces() const { return faces; }
        };
    }
}

#endif //RAYTRACER_MESH_HPP
