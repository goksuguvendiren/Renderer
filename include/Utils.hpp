//
// Created by Göksu Güvendiren on 2018-10-27.
//

#pragma once

#include <sstream>
#include <vector>
#include <tinyxml/tinyxml2.h>

namespace gpt
{
    namespace utils
    {

        inline int GetInt(std::istringstream& stream)
        {
            int val;
            stream >> val;
            return val;
        }

        inline int GetInt(tinyxml2::XMLElement* element)
        {
            std::istringstream stream{element->GetText()};

            int val;
            stream >> val;
            return val;
        }

        inline auto GetTransformations(std::istringstream& stream)
        {
            std::vector<std::string> result;

            while(stream.good())
            {
                std::string tr;
                stream >> tr;
                result.push_back(tr);
            }

            return result;
        }

        inline auto GetTransformations(tinyxml2::XMLElement* element)
        {
            std::istringstream stream {element->GetText()};

            std::vector<std::string> result;

            while(stream.good())
            {
                std::string tr;
                stream >> tr;
                result.push_back(tr);
            }

            return result;
        }

        inline glm::vec3 GetVertex(std::istringstream& stream)
        {
            glm::vec3 vert;

            float datax;
            float datay;
            float dataz;

            stream >> datax;
            stream >> datay;
            stream >> dataz;

            return glm::vec3{datax, datay, dataz};
        }

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