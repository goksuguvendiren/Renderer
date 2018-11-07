//
// Created by Göksu Güvendiren on 20/07/2018.
//

#pragma once

#include <Ray.hpp>

namespace gpt
{
    class Shape;
    class Material;

    class HitInfo
    {
        float param;
        glm::vec3 normal;
        glm::vec3 position;
        Ray r;

        const gpt::Shape* shape;
    public:
        HitInfo(glm::vec3 n, glm::vec3 point, Ray ry, const gpt::Shape* hitShape, float t = std::numeric_limits<float>::infinity()) : param(t),
                                                                                                          normal(n),
                                                                                                          position(point),
                                                                                                          r(ry),
                                                                                                          shape(hitShape)
        {}

        float       Param() const { return param; }
        glm::vec3   Normal() const { return normal; }
        glm::vec3   Position() const { return position; }

        const gpt::Ray& GetRay() const { return r; }

        const gpt::Shape* Shape() const { return shape; }
        const gpt::Material&  Material() const;
    };
}

