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
        inline float sample_float()
        {
            static std::mt19937 float_seed;
            static std::uniform_real_distribution<float> generator;

            return generator(float_seed);
        }

        inline glm::vec3 sample_hemisphere(float sample1, float sample2)
        {
            auto radius = std::sqrt(sample1);
            auto theta  = 2 * glm::pi<float>() * sample2;

            auto x = radius * glm::cos(theta);
            auto y = radius * glm::sin(theta);

            return glm::vec3(x, y, std::sqrt(std::max(0.0f, 1 - sample1)));
        }

        inline glm::vec3 sample_hemisphere(const glm::vec3& normal)
        {
            static std::mt19937 hemisphere_seed;

            static std::uniform_real_distribution<float> distribution1(0, 1);
            static std::uniform_real_distribution<float> distribution2(0, 1);
            float sample1 = distribution1(hemisphere_seed);
            float sample2 = distribution2(hemisphere_seed);

            auto baseSample = sample_hemisphere(sample1, sample2);

            if (normal == glm::vec3{0, 0, -1})
            {
                return -baseSample;
            }

            auto axis = glm::cross(glm::vec3(0, 0, 1), normal);
            auto angle = std::acos(glm::dot(normal, glm::vec3(0, 0, 1)));

            auto rotate = glm::angleAxis(angle, axis);
            return rotate * baseSample;
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