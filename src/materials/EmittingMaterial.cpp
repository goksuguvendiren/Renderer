//
// Created by Göksu Güvendiren on 2018-10-27.
//

#include "materials/EmittingMaterial.hpp"

glm::vec3 gpt::materials::EmittingMaterial::CalculateReflectance(const glm::vec3& incoming, const glm::vec3& outgoing, const glm::vec3& normal) const
{
    return radiance;
}