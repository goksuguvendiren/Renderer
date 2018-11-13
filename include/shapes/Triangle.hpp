//
// Created by Göksu Güvendiren on 20/07/2018.
//

#pragma once

#include <vector>
#include <tinyxml/tinyxml2.h>
#include "shapes/Shape.hpp"

namespace gpt
{
    class Scene;
    namespace shapes
    {
        class Triangle : public Shape
        {
            glm::vec3 pointA;
            glm::vec3 pointB;
            glm::vec3 pointC;

            glm::vec3 surfNormal;

        public:
            Triangle(int id, const glm::vec3 &a, glm::vec3 b, glm::vec3 c, const gpt::Material &m, int tid = -1,
                     int tr_id = 1) : Shape(m, id), pointA(a), pointB(b), pointC(c)
            {
                surfNormal = glm::normalize(glm::cross(pointB - pointA, pointC - pointA));
            }

//            Triangle(const glm::vec3 &pointA);

            ~Triangle() override = default;

            boost::optional<HitInfo> Hit (const Ray& ray) const override;
//            boost::optional<float>   ShadowHit(const Ray& ray) const;

            auto Normal() const { return surfNormal; }

//            glm::vec3& PointA() { return pointA; }
//            glm::vec3& PointB() { return pointB; }
//            glm::vec3& PointC() { return pointC; }

            glm::vec3 PointA() const { return pointA; }
            glm::vec3 PointB() const { return pointB; }
            glm::vec3 PointC() const { return pointC; }

            bool isArtificial() const { return false; }
        };
    }
}
