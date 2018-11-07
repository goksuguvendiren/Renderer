//
// Created by Göksu Güvendiren on 2018-11-02.
//

#include <materials/BasicMaterial.hpp>
#include <HitInfo.hpp>
#include <lights/Light.hpp>

//glm::vec3 Fund(const gpt::HitInfo& hit, const gpt::Light& light, glm::vec3 diffuse)
//{
//    auto direction = light.Direction(hit.Position());
//    auto intensity = light.Intensity(direction);
//
////    glm::vec3 color = {0, 0, 0};
//    glm::vec3 color = diffuse;
//
//    // Diffuse shading :
//    auto theta = std::max(0.f, glm::dot(glm::normalize(hit.Normal()), glm::normalize(direction)));
//    color += theta * color * intensity;
//
//    // Specular shading :
////    auto half = glm::normalize(glm::normalize(direction) + glm::normalize(hit.Ray().Origin() - hit.Position()));
////    color += intensity * specular * std::pow(std::max(glm::dot(half, hit.Normal()), 0.0f), phong_exp);
//
//    return color;
//}

glm::vec3 gpt::materials::BasicMaterial::DiffuseColor(const gpt::HitInfo& hit, glm::vec3 direction, glm::vec3 intensity) const
{
    glm::vec3 color = diffuse;
    auto theta = std::max(0.f, glm::dot(glm::normalize(hit.Normal()), glm::normalize(direction)));
    color += theta * color * intensity;

    return color;
}

//glm::vec3 gpt::materials::BasicMaterial::SpecularColor(const gpt::HitInfo& hit, glm::vec3 direction, glm::vec3 intensity)
//{
//    glm::vec3 color = {};
//    auto half = glm::normalize(glm::normalize(direction) + glm::normalize(hit.HitRay().Origin() - hit.Position()));
//    color += intensity * specular * std::pow(std::max(glm::dot(half, hit.Normal()), 0.0f), phong_exp);
//
//    return color;
//}

glm::vec3 gpt::materials::BasicMaterial::CalculateReflectance(const Scene &scene, const gpt::HitInfo& hit, int recdepth) const
{
    glm::vec3 color = ambient * scene.AmbientColor();

    for (auto& light : scene.Lights())
    {
        auto direction = light->Direction(hit.Position());
        gpt::Ray shadowRay(hit.Position() + (glm::vec3{1e-3, 1e-3, 1e-3} * glm::normalize(direction)), glm::normalize(direction));

        boost::optional<HitInfo> sh;
        if ((sh = scene.Hit(shadowRay)))
        {
            if (sh->Param() < glm::length(direction))
            continue;
        }

        auto intensity = light->Intensity(direction);
//
        color += DiffuseColor(hit, direction, intensity);
//        color += SpecularColor(hit)
    }

    return color;

}
