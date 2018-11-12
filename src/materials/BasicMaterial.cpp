//
// Created by Göksu Güvendiren on 2018-11-02.
//

#include <materials/BasicMaterial.hpp>
#include <HitInfo.hpp>
#include <lights/Light.hpp>
#include <glm/gtc/constants.hpp>

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

glm::vec3 gpt::materials::BasicMaterial::CalculateReflectance(const glm::vec3& incoming, const glm::vec3& outgoing, const glm::vec3& normal) const
{
//    glm::vec3 color = ambient * scene.AmbientColor();
//
//    for (auto& light : scene.Lights())
//    {
//        auto direction = light->Direction(hit.Position());
//        gpt::Ray shadowRay(hit.Position() + (glm::vec3{1e-3, 1e-3, 1e-3} * glm::normalize(direction)), glm::normalize(direction));
//
//        boost::optional<HitInfo> sh;
//        if ((sh = scene.Hit(shadowRay)))
//        {
//            if (sh->Param() < glm::length(direction))
//            continue;
//        }
//
//        auto intensity = light->Intensity(direction);
////
//        color += DiffuseColor(hit, direction, intensity);
////        color += SpecularColor(hit)
//    }
//
//    return color;

    glm::vec3 w_i  = glm::normalize(incoming);
    glm::vec3 w_o  = glm::normalize(outgoing);
    glm::vec3 half = glm::normalize(w_i + w_o);

    float theta_h   = std::max(0.f, glm::dot(half, normal));
//    float theta_i = glm::dot(hit.Normal(), w_i);

    glm::vec3 diff_term = diffuse / glm::pi<float>();
    glm::vec3 spec_term = (specular * std::pow(theta_h, 100.f)) / glm::pi<float>();

//    if (normalized)
//    {
//        spec_term *= (exponent + 8.f) / 8.f;
//        spec_term /= glm::pi<float>();
//        diff_term /= glm::pi<float>();
//    }

    glm::vec3 color = diff_term + spec_term;

    return color;// * theta_i;

}
