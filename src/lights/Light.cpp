//
// Created by Göksu Güvendiren on 2018-10-27.
//

#include <lights/Light.hpp>
#include <lights/PointLight.hpp>
#include <vector>
#include <tinyxml/tinyxml2.h>

std::vector<std::unique_ptr<gpt::lights::Light>> gpt::lights::Light::Load(gpt::Scene &scene, tinyxml2::XMLElement *elem)
{
    std::vector<std::unique_ptr<gpt::lights::Light>> lights;
    for (auto child = elem->FirstChildElement("PointLight"); child != nullptr; child = child->NextSiblingElement("PointLight"))
    {
        int id;
        child->QueryIntAttribute("id", &id);

        auto position = utils::GetElem(child->FirstChildElement("Position"));
        auto intensity = utils::GetElem(child->FirstChildElement("Intensity"));

        lights.push_back(std::make_unique<gpt::lights::PointLight>(id, position, intensity));
    }
    return lights;
}