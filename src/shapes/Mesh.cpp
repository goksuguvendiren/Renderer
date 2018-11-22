//
// Created by Göksu Güvendiren on 21/07/2018.
//

#include <shapes/Mesh.hpp>
#include "Scene.hpp"
#include "Utils.hpp"

boost::optional<gpt::HitInfo> gpt::shapes::Mesh::Hit(const Ray &ray) const
{
    auto hit = aabb.Hit(ray);
    hit->Shape(this);
    return hit;
}

