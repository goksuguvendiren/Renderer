//
// Created by Göksu Güvendiren on 2018-10-27.
//

#pragma once


#include <glm/vec3.hpp>
#include <map>
#include <tinyxml/tinyxml2.h>
#include <Utils.hpp>
#include "Material.hpp"

namespace gpt
{
    class Scene;

    namespace materials
    {
        class EmittingMaterial : public Material
        {
            glm::vec3 radiance;

        public:
            ~EmittingMaterial() override = default;
            explicit EmittingMaterial(const glm::vec3 &rad) : Material(true), radiance(rad) {}

            glm::vec3 CalculateReflectance(const glm::vec3& incoming, const glm::vec3& outgoing, const glm::vec3& normal) const override;
        };
    }
}