//
// Created by Göksu Güvendiren on 2018-10-27.
//

#pragma once

#include <sstream>
#include <vector>
#include <tinyxml/tinyxml2.h>

#include <random>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/constants.hpp>

namespace gpt
{
    namespace utils
    {
        inline glm::vec3 sample_hemisphere(const glm::vec3& normal)
        {
            static std::mt19937 hemisphere_seed;

            static std::uniform_real_distribution<float> asd(0, 1);
            static std::uniform_real_distribution<float> asd1(0, 1);
            float sample1 = asd(hemisphere_seed);
            float sample2 = asd1(hemisphere_seed);

            auto dir = glm::vec3{cos(2 * glm::pi<float>() * sample2) * glm::sqrt(1 - glm::pow(sample1, 2.f)),
                                 sample1,
                                 sin(2 * glm::pi<float>() * sample2) * glm::sqrt(1 - glm::pow(sample2, 2.f))};

            auto c = glm::cross(glm::vec3{0.f, 1.f, 0.f}, normal);
            auto angle = glm::acos(glm::dot(glm::vec3{0, 1, 0}, normal));
            auto res = glm::angleAxis(angle, c) * glm::normalize(dir);

            const float r = std::sqrt(sample1);
            const float theta = 2 * glm::pi<float>() * sample2;

            const float x = r * std::cos(theta);
            const float y = r * std::sin(theta);

            return glm::normalize(glm::vec3(x, y, std::sqrt(std::max(0.0f, 1 - sample1))));

            if (normal == glm::vec3{0, -1, 0})
            {
                return -res;
            }
            return res;
        }

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