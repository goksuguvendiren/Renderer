//
// Created by Göksu Güvendiren on 2018-10-27.
//

#pragma once

#include <glm/vec3.hpp>
#include <map>
#include <tinyxml/tinyxml2.h>
#include <HitInfo.hpp>
//#include <Scene.hpp>

namespace gpt
{
    struct MaterialLoadContext
    {

    };

    class Scene;

    class Material
    {
    public:
        virtual glm::vec3 CalculateReflectance(const Scene &scene, const gpt::HitInfo& hit, int recdepth) const = 0;
        virtual ~Material() = default;
    };
}