//
// Created by Göksu Güvendiren on 2018-10-27.
//

#include <materials/Material.hpp>
#include <Scene.hpp>

static glm::vec3 GetElem(tinyxml2::XMLElement* element)
{
    glm::vec3 color;

    std::istringstream ss {element->GetText()};
    ss >> color.r;
    ss >> color.g;
    ss >> color.b;

    return color;
}

std::map<int, gpt::materials::Material> gpt::materials::Material::Load(gpt::Scene &scene, tinyxml2::XMLElement *elem)
{
    std::map<int, gpt::materials::Material> materials;
    for (auto child = elem->FirstChildElement("Material"); child != nullptr; child = child->NextSiblingElement())
    {
        int id;
        child->QueryIntAttribute("id", &id);

//        tinyxml2::XMLElement* tmp;
//        if ((tmp = child->FirstChildElement("LightSource"))) // then the material should be a glass (transparent)
//        {
//            std::string name = "/Users/goksu/Documents/METU/AdvancedRayTracer/";
//            name += tmp->GetText();
//
//            auto material = new LightSource(id, name);
//            mats.push_back(material);
//            continue;
//        }
//
//        int brdf_id = -1;
//        if (child->QueryIntAttribute("BRDF", &brdf_id));
//
        auto  ambient  = GetElem(child->FirstChildElement("AmbientReflectance"));
        auto  diffuse  = GetElem(child->FirstChildElement("DiffuseReflectance"));
        auto  specular = GetElem(child->FirstChildElement("SpecularReflectance"));
//
//
//        glm::vec3  mirror = {0, 0, 0}, transparency = {0, 0, 0}, glass = {0, 0, 0};
//        float phongEx = 0;
//
//        if ((tmp = child->FirstChildElement("PhongExponent")))
//            phongEx  = tmp->FloatText(1);
//
//        auto mtrl = new ClassicMaterial(id, ambient, diffuse, specular, phongEx, brdf_id);
//
//        // Create some classical material with this information
//        // Then bind this material with the mirror / transparent / brdf if one exists.
//
//        if ((tmp = child->FirstChildElement("MirrorReflectance"))) // then the material should be a mirror
//        {
//            tinyxml2::XMLElement* tmp_m;
//            mirror = GetElem(tmp);
//            float roughness = 0;
//            if ((tmp_m = child->FirstChildElement("Roughness"))) {
//                roughness = tmp_m->FloatText(0);
//            }
//
//            auto material = new Mirror(id, mirror, roughness);
//            mats.push_back(material);
//            continue;
//        }
//
//        if ((tmp = child->FirstChildElement("Transparency"))) // then the material should be a glass (transparent)
//        {
//            tinyxml2::XMLElement* tmp_m;
//            glass = GetElem(tmp);
//            float refraction_index = 1.f;
//            if ((tmp_m = child->FirstChildElement("RefractionIndex"))) {
//                refraction_index = tmp_m->FloatText(1.f);
//            }
//
//            auto material = new Glass(id, refraction_index, glass);
//            mats.push_back(material);
//            continue;
//        }
//
        materials.insert(std::make_pair(id, Material(ambient, diffuse, specular)));
    }

    return materials;
}

glm::vec3 gpt::materials::Material::CalculateReflectance(const Scene &scene) const
{
    glm::vec3 color = ambient * scene.AmbientColor();

//    for (auto& light : scene.Lights())
//    {
//        auto direction = light->Direction(hit.Position());
//        Ray shadowRay(hit.Position() + (scene.ShadowRayEpsilon() * glm::normalize(direction)),
//                      glm::normalize(direction));

//        boost::optional<float> sh;
//        if ((sh = scene.ShadowHit(shadowRay)))
//        {
//            if (*sh < glm::length(direction))
//                continue;
//        }

//        color += ComputeReflectance(hit, *light);
//    }

    return color;
}
