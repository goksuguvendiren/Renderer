//
// Created by Göksu Güvendiren on 2018-10-27.
//

#pragma once

#include <glm/vec3.hpp>
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

            glm::vec3 Direction(const glm::vec3& hitpos) const;
        };
    }
}