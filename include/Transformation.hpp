//
// Created by Göksu Güvendiren on 22/07/2018.
//

#ifndef RAYTRACER_TRANSFORMATION_HPP
#define RAYTRACER_TRANSFORMATION_HPP

#include <string>
#include <glm/detail/type_mat.hpp>
#include <tinyxml/tinyxml2.h>
#include <map>

namespace gpt
{
    enum class Transformations
    {
        Translation,
        Scaling,
        Rotation
    };

    std::map<std::string, glm::mat4> LoadTransformations(tinyxml2::XMLElement *elem);
    std::map<std::string, glm::mat4> LoadTranslations(tinyxml2::XMLElement *elem);
    std::map<std::string, glm::mat4> LoadRotations(tinyxml2::XMLElement *elem);
    std::map<std::string, glm::mat4> LoadScalings(tinyxml2::XMLElement *elem);
}

#endif //RAYTRACER_TRANSFORMATION_HPP
