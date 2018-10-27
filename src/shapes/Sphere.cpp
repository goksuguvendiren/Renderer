//
// Created by Göksu Güvendiren on 20/07/2018.
//

#include <shapes/Sphere.hpp>
#include <tinyxml/tinyxml2.h>
#include <string>
#include <sstream>
#include "Scene.hpp"

boost::optional<gpt::HitInfo> gpt::shapes::Sphere::Hit(const Ray &ray) const
{
    auto inverseRay = Ray{glm::vec3(inverse_transf * glm::vec4(ray.Origin(), 1)),
                          glm::vec3(inverse_transf * glm::vec4(ray.Direction(), 0))};

    auto eminc = inverseRay.Origin() - center;

    auto A = glm::dot(inverseRay.Direction(), inverseRay.Direction());
    auto B = 2.0f * glm::dot(inverseRay.Direction(), eminc);
    auto C = glm::dot(eminc, eminc) - radius * radius;

    auto delta = B * B - 4 * A * C;

    if (delta < 1e-4) return boost::none;

    auto param = (- B - std::sqrt(delta)) / (2.0f * A);

    if (param < 0)
    {
        param = (- B + std::sqrt(delta)) / (2.0f * A);

        if (param < 0)
            return boost::none;
    }

    auto modelPoint    = inverseRay.Origin() + param * inverseRay.Direction();
    auto surfaceNormal = glm::normalize(glm::vec3(inverse_transpose_transf * glm::vec4(modelPoint - center, 0)));
    auto hitPoint      = glm::vec3(transformation_matrix * glm::vec4(modelPoint, 1));

    if (std::isinf(param) || std::isnan(param))
    {
        return boost::none;
    }

    return HitInfo(surfaceNormal, hitPoint, ray, this, param);
}

std::vector<gpt::shapes::Sphere> gpt::shapes::Sphere::Load(gpt::Scene &scene, tinyxml2::XMLElement *elem)
{
    std::vector<gpt::shapes::Sphere> spheres;

    for (auto child = elem->FirstChildElement("Sphere"); child != nullptr; child = child->NextSiblingElement("Sphere")) {
        int id;
        child->QueryIntAttribute("id", &id);
        int materialID  = child->FirstChildElement("Material")->IntText(0);
        int centerID    = child->FirstChildElement("Center")->IntText(0);
        float radius    = child->FirstChildElement("Radius")->FloatText(0);

        glm::vec3 center = scene.GetVertex(centerID);

        std::vector<std::string> transformations;
        if(auto trns = child->FirstChildElement("Transformations"))
        {
            std::istringstream ss {trns->GetText()};
            transformations = gpt::GetTransformations(ss);
        }

        glm::mat4 matrix(1.0f);
        for (auto& tr : transformations)
        {
            auto m = scene.GetTransformation(tr);
            matrix = m * matrix;
        }

        int tid = -1;
        if (auto tex = child->FirstChildElement("Texture"))
        {
            tid = tex->IntText(-1);
        }

        Sphere sp {id, radius, center, scene.GetMaterial(materialID)};
        sp.TransformationMatrix(matrix);

        spheres.push_back(std::move(sp));
    }

    return spheres;
}