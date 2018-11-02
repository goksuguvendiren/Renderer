//
// Created by Göksu Güvendiren on 2018-10-27.
//

#pragma once

#include <glm/vec3.hpp>
#include <vector>
#include <tinyxml/tinyxml2.h>

#include "Utils.hpp"

namespace gpt
{
    class Light
    {
    public:
        virtual glm::vec3 Direction(const glm::vec3& hitpos) const = 0;
//        //virtual glm::vec3 Position() const = 0;
//        virtual glm::vec3 Intensity(const glm::vec3& lightPos_hitPos) const = 0;

        static std::vector<std::unique_ptr<gpt::Light>> Load(tinyxml2::XMLElement *elem);
    };
}