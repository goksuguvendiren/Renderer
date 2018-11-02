//
// Created by Göksu Güvendiren on 2018-11-01.
//

#pragma once

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
            BasicMaterial() : ambient({0, 0, 0}), diffuse({0, 0, 0}), specular({0, 0, 0}) {}
            BasicMaterial(const glm::vec3 &amb, const glm::vec3 &dif, const glm::vec3 &spe) : ambient(amb), diffuse(dif), specular(spe)
            {}

            glm::vec3 CalculateReflectance(const Scene &scene, const gpt::HitInfo& hit, int recdepth) const override
            {
                glm::vec3 color = ambient * scene.AmbientColor();

                for (auto& light : scene.Lights())
                {
                    auto direction = light->Direction(hit.Position());
                    gpt::Ray shadowRay(hit.Position() + (scene.ShadowRayEpsilon() * glm::normalize(direction)), glm::normalize(direction));

        //          boost::optional<float> sh;
        //          if ((sh = scene.ShadowHit(shadowRay)))
        //          {
        //                if (*sh < glm::length(direction))
        //                continue;
        //          }

        //          color += ComputeReflectance(hit, *light);
                }

                return color;

            }
        };
    }
}