//
// Created by Göksu Güvendiren on 22/07/2018.
//

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <string>
#include <sstream>
#include <tinyxml/tinyxml2.h>
#include <map>

#include "Transformation.hpp"

static glm::vec3 GetElem(std::istringstream& ss)
{
    glm::vec3 color;
    ss >> color.r;
    ss >> color.g;
    ss >> color.b;

    return color;
}

static float GetAngle(std::istringstream& ss)
{
    float angle;
    ss >> angle;

    return angle;
}

std::map<std::string, glm::mat4> gpt::LoadTransformations(tinyxml2::XMLElement *elem)
{
    std::map<std::string, glm::mat4> trs;

    auto translations = gpt::LoadTranslations(elem);
    for (auto translation : translations){
        trs.insert(translation);
    }

    auto rotations = gpt::LoadRotations(elem);
    for (auto rotation : rotations){
        trs.insert(rotation);
    }

    auto scalings = gpt::LoadScalings(elem);
    for (auto scaling : scalings){
        trs.insert(scaling);
    }

    return trs;
}


std::map<std::string, glm::mat4> gpt::LoadTranslations(tinyxml2::XMLElement *elem)
{
    std::map<std::string, glm::mat4> trs;
    for (auto child = elem->FirstChildElement("Translation"); child != nullptr; child = child->NextSiblingElement()){
        int id;
        child->QueryIntAttribute("id", &id);

        std::istringstream ss {child->GetText()};
        auto details = GetElem(ss);
        auto View = glm::translate(glm::mat4(1.), details);

        auto sth = "t" + std::to_string(id);
        trs.insert(std::make_pair(sth, View));
    }

    return trs;
}


std::map<std::string, glm::mat4> gpt::LoadRotations(tinyxml2::XMLElement *elem)
{
    std::map<std::string, glm::mat4> rts;
    for (auto child = elem->FirstChildElement("Rotation"); child != nullptr; child = child->NextSiblingElement()){
        int id;
        child->QueryIntAttribute("id", &id);

        std::istringstream ss {child->GetText()};

        auto angle   = GetAngle(ss);
        auto details = GetElem(ss);
        auto matrix  = glm::rotate(glm::mat4(1.), glm::radians(angle), details);

        auto sth = "r" + std::to_string(id);
        rts.insert(std::make_pair(sth, matrix));
    }

    return rts;
}

std::map<std::string, glm::mat4> gpt::LoadScalings(tinyxml2::XMLElement *elem)
{
    std::map<std::string, glm::mat4> scls;
    for (auto child = elem->FirstChildElement("Scaling"); child != nullptr; child = child->NextSiblingElement()){
        int id;
        child->QueryIntAttribute("id", &id);

        std::istringstream ss {child->GetText()};
        auto details = GetElem(ss);
        auto matrix = glm::scale(glm::mat4(1.), details);

        auto sth = "s" + std::to_string(id);
        scls.insert(std::make_pair(sth, matrix));
    }

    return scls;
}
