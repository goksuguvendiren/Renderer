//
// Created by Göksu Güvendiren on 2018-10-27.
//

#pragma once

#include <sstream>
#include <tinyxml/tinyxml2.h>

namespace gpt
{
    namespace utils
    {
        inline glm::vec3 GetElem(std::istringstream& ss)
        {
            glm::vec3 color;

            ss >> color.r;
            ss >> color.g;
            ss >> color.b;

            return color;
        }

        inline glm::vec3 GetElem(tinyxml2::XMLElement* element)
        {
            std::istringstream ss {element->GetText()};

            glm::vec3 color;
            ss >> color.r;
            ss >> color.g;
            ss >> color.b;

            return color;
        }
    }
}