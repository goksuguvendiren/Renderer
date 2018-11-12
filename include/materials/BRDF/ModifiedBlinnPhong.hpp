//
// Created by Göksu Güvendiren on 2018-11-02.
//

#pragma once

#include <materials/BasicMaterial.hpp>


#include <glm/vec3.hpp>
#include <map>
#include <tinyxml/tinyxml2.h>

namespace gpt
{
    class Scene;

    namespace materials
    {
        class ModifiedBlinnPhong : public Material
        {
            BasicMaterial basic;
            float exponent;

        public:
            ModifiedBlinnPhong() : basic({0, 0, 0}, {0, 0, 0}, {0, 0, 0}), exponent(0) {}
            ModifiedBlinnPhong(const BasicMaterial& basicMaterial, float exp) : basic(basicMaterial), exponent(exp) {}
            ModifiedBlinnPhong(const glm::vec3 &amb, const glm::vec3 &dif, const glm::vec3 &spe, float exp) : basic(amb, dif, spe), exponent(exp)
            {}
            ~ModifiedBlinnPhong() override = default;

//            glm::vec3 DiffuseColor(const gpt::HitInfo& hit, glm::vec3 direction, glm::vec3 intensity) const;
//            glm::vec3 SpecularColor(const gpt::HitInfo& hit, glm::vec3 direction, glm::vec3 intensity) const ;

//            glm::vec3 CalculateReflectance(const gpt::HitInfo& hit, const gpt::Light& light, int recdepth) const override;
            glm::vec3 CalculateReflectance(const glm::vec3& incoming, const glm::vec3& outgoing, const glm::vec3& normal) const override;

        };
    }
}