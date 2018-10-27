//
// Created by Göksu Güvendiren on 2018-10-27.
//

#pragma once

#include <glm/vec3.hpp>
#include <map>
#include <tinyxml/tinyxml2.h>
//#include <Scene.hpp>

namespace gpt
{
    class Scene;

namespace materials
{
    class Material
    {
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

    public:
        Material(const glm::vec3 &amb, const glm::vec3 &dif, const glm::vec3 &spe) : ambient(amb), diffuse(dif),
                                                                                     specular(spe)
        {}

        glm::vec3 CalculateReflectance(const Scene &scene) const;

        static std::map<int, Material> Load(gpt::Scene &scene, tinyxml2::XMLElement *elem);
    };
}
}