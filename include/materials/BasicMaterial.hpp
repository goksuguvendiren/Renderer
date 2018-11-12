//
// Created by Göksu Güvendiren on 2018-10-27.
//

#pragma once


#include <glm/vec3.hpp>
#include <map>
#include <tinyxml/tinyxml2.h>
#include "Material.hpp"
#include "Scene.hpp"

namespace gpt
{
    class Scene;

    namespace materials
    {
        class BasicMaterial : public Material
        {
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;

        public:
            BasicMaterial() : ambient({0, 0, 0}), diffuse({0, 0, 0}), specular({0, 0, 0}), Material(false) {}
            BasicMaterial(const glm::vec3 &amb, const glm::vec3 &dif, const glm::vec3 &spe) : ambient(amb), diffuse(dif), specular(spe), Material(false)
            {}
            ~BasicMaterial() override = default;

            glm::vec3 Diffuse() const { return diffuse; }
            glm::vec3 Specular() const { return specular; }

            glm::vec3 DiffuseColor(const gpt::HitInfo& hit, glm::vec3 direction, glm::vec3 intensity) const;
            glm::vec3 SpecularColor(const gpt::HitInfo& hit, glm::vec3 direction, glm::vec3 intensity) const ;

            glm::vec3 CalculateReflectance(const glm::vec3& incoming, const glm::vec3& outgoing, const glm::vec3& normal) const override;
        };
    }
}