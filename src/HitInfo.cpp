//
// Created by Göksu Güvendiren on 2018-10-27.
//

#include <shapes/Shape.hpp>
#include <HitInfo.hpp>

const gpt::materials::Material* gpt::HitInfo::Material() const
{
     return shape->Material();
}

