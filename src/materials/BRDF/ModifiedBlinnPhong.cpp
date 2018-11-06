//
// Created by Göksu Güvendiren on 2018-11-02.
//

#include <materials/BRDF/ModifiedBlinnPhong.hpp>

glm::vec3 gpt::materials::ModifiedBlinnPhong::CalculateReflectance(const gpt::HitInfo& hit, const gpt::Light& light) const
{
    auto direction = light.Direction(hit.Position()); //point to light direction

    glm::vec3 w_i  = glm::normalize(direction);
    glm::vec3 w_o  = glm::normalize(-hit.HitRay().Direction());
    glm::vec3 half = glm::normalize(w_i + w_o);

    float alpha   = std::max(0.f, glm::dot(half, hit.Normal()));
    float theta_i = glm::dot(hit.Normal(), w_i);

    glm::vec3 diff_term = material.Diffuse();
    glm::vec3 spec_term = (material.Specular() * std::pow(alpha, exponent));

    if (normalized)
    {
        spec_term *= (exponent + 8.f) / 8.f;
        spec_term /= glm::pi<float>();
        diff_term /= glm::pi<float>();
    }

    glm::vec3 color = diff_term + spec_term;

    return color * theta_i;
}