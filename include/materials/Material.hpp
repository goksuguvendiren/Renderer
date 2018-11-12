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
        bool terminate = false;

    protected:
        Material() = default;
        explicit Material(bool term) : terminate(term) {}
    public:
        virtual glm::vec3 CalculateReflectance(const glm::vec3& incoming, const glm::vec3& outgoing, const glm::vec3& normal) const = 0;
        virtual ~Material() = default;
        bool Terminate() const { return terminate; }
    };
}