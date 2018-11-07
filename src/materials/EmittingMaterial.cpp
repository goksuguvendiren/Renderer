//
// Created by Göksu Güvendiren on 2018-10-27.
//

#include "materials/EmittingMaterial.hpp"

glm::vec3 gpt::materials::EmittingMaterial::CalculateReflectance(const Scene &scene, const gpt::HitInfo& hit, int recdepth) const
{
    return radiance;
}