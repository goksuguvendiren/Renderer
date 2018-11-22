//
// Created by Göksu Güvendiren on 20/07/2018.
//

#pragma once

#include <vector>
#include <tinyxml/tinyxml2.h>
#include <AABB.hpp>
#include "shapes/Shape.hpp"
#include <type_traits>

namespace gpt
{
    class Scene;
    class Triangle
    {
        int id;
        glm::vec3 pointA;
        glm::vec3 pointB;
        glm::vec3 pointC;

        glm::vec3 surfNormal;

    public:
        Triangle(int id, const glm::vec3 &a, const glm::vec3 & b, const glm::vec3 & c, const gpt::Material &m, int tid = -1,
                 int tr_id = 1) : id(id), pointA(a), pointB(b), pointC(c)
        {
            surfNormal = glm::normalize(glm::cross(pointB - pointA, pointC - pointA));
        }

        boost::optional<HitInfo> Hit (const Ray& ray) const;

        auto Normal() const { return surfNormal; }

        glm::vec3 PointA() const { return pointA; }
        glm::vec3 PointB() const { return pointB; }
        glm::vec3 PointC() const { return pointC; }

        bool isArtificial() const { return false; }
    };

//    static_assert(std::is_nothrow_move_constructible<shapes::Triangle>{}, "Triangle must be nothrow move constructible");
}
