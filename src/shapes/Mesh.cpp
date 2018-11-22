//
// Created by Göksu Güvendiren on 21/07/2018.
//

#include <shapes/Mesh.hpp>
#include "Scene.hpp"
#include "Utils.hpp"

boost::optional<gpt::HitInfo> gpt::shapes::Mesh::Hit(const Ray &ray) const
{
//    boost::optional<gpt::HitInfo> min_hit = boost::none;
//    for (auto& face : faces)
//    {
//        auto hit = face.Hit(ray);
//        if (!hit) continue;
//        if (!min_hit || hit->Param() < min_hit->Param())
//        {
//            min_hit = *hit;
//        }
//    }
//
//    if (min_hit)
//        min_hit->Shape(this);
//
//    return min_hit;

    return aabb.Hit(ray);
}

