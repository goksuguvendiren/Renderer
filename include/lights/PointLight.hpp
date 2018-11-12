//
// Created by Göksu Güvendiren on 2018-10-27.
//

#pragma once

#include <glm/glm.hpp>
#include "Light.hpp"

namespace gpt
{
    namespace lights
    {
        class PointLight : public Light
        {
            glm::vec3 position;
            glm::vec3 intensity;

        public:
            PointLight(const glm::vec3& pos, const glm::vec3& intens) : position(pos), intensity(intens) {}

            glm::vec3 Direction(const glm::vec3& hitpos) const { return position - hitpos; }
            glm::vec3 Intensity(const glm::vec3& lightPos_hitPos) const
            {
                return intensity / glm::dot(lightPos_hitPos, lightPos_hitPos);
            }
        };
    }
}