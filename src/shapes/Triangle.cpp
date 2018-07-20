//
// Created by Göksu Güvendiren on 20/07/2018.
//

#include "shapes/Triangle.hpp"


inline float determinant(const glm::vec3& col1,
                         const glm::vec3& col2,
                         const glm::vec3& col3)
{
    return col1.x * (col2.y * col3.z - col2.z * col3.y) -
           col2.x * (col1.y * col3.z - col1.z * col3.y) +
           col3.x * (col1.y * col2.z - col1.z * col2.y);

}

boost::optional<gpt::HitInfo> gpt::shapes::Triangle::Hit (const gpt::Ray& ray) const
{
    glm::vec3 col1 = pointA - pointB;
    glm::vec3 col2 = pointA - pointC;
    glm::vec3 col3 = ray.Direction();
    glm::vec3 col4 = pointA - ray.Origin();

    auto detA  = determinant(col1, col2, col3);
    if (detA == 0) return boost::none;

    auto beta  = determinant(col4, col2, col3) / detA;
    auto gamma = determinant(col1, col4, col3) / detA;
    auto param = determinant(col1, col2, col4) / detA;
    auto alpha = 1 - beta - gamma;

    if (alpha < -0.0001 || gamma < -0.0001 || beta < -0.0001 || param < -0.0001) return boost::none;

    auto point = ray.Origin() + param * ray.Direction();

    glm::vec3 normal = glm::normalize(alpha * surfNormal + beta * surfNormal + gamma * surfNormal);

    return gpt::HitInfo(normal, point, ray, param);
}

gpt::shapes::Triangle::Triangle(int i, glm::vec3 a, glm::vec3 b, glm::vec3 c, int tid, int tr_id)
{
    id = i;
    pointA = a;
    pointB = b;
    pointC = c;

    surfNormal = glm::normalize(glm::cross(pointB - pointA, pointC - pointA));
}