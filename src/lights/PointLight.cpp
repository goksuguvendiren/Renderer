//
// Created by Göksu Güvendiren on 2018-10-27.
//

#include <lights/PointLight.hpp>

glm::vec3 gpt::lights::PointLight::Direction(const glm::vec3 &hitpos) const
{
    return position - hitpos;
}
