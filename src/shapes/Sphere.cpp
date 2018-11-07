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
