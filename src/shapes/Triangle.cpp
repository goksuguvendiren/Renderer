//
// Created by Göksu Güvendiren on 20/07/2018.
//

#include <sstream>
#include "shapes/Triangle.hpp"
#include "Scene.hpp"

inline float determinant(const glm::vec3& col1,
                         const glm::vec3& col2,
                         const glm::vec3& col3)
{
    return col1.x * (col2.y * col3.z - col2.z * col3.y) -
           col2.x * (col1.y * col3.z - col1.z * col3.y) +
           col3.x * (col1.y * col2.z - col1.z * col2.y);

}

inline bool isBackFace(const glm::vec3& surfaceNormal, const glm::vec3& direction)
{
    return glm::dot(surfaceNormal, direction) < 0;
}

boost::optional<gpt::HitInfo> gpt::Triangle::Hit(const gpt::Ray& ray) const
{
    glm::vec3 col1 = pointA - pointB;
    glm::vec3 col2 = pointA - pointC;
    glm::vec3 col3 = ray.Direction();
    glm::vec3 col4 = pointA - ray.Origin();

    auto epsilon = -1e-4;
    auto detA  = determinant(col1, col2, col3);
    if (detA == 0) return boost::none;

    auto oneOverDetA = 1 / detA;

    auto beta  = determinant(col4, col2, col3) * oneOverDetA;
    auto gamma = determinant(col1, col4, col3) * oneOverDetA;
    auto param = determinant(col1, col2, col4) * oneOverDetA;
    auto alpha = 1 - beta - gamma;

    if (alpha < epsilon || gamma < epsilon|| beta < epsilon || param < epsilon)
    {
        return boost::none;
    }
    if (ray.IsPrimary() && !isBackFace(surfNormal, ray.Direction()))
    {
        return boost::none;
    }

    auto point = ray.Origin() + param * ray.Direction();
    glm::vec3 normal = glm::normalize(alpha * surfNormal + beta * surfNormal + gamma * surfNormal);

    return gpt::HitInfo(normal, point, ray, nullptr, param);
}