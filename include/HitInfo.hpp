//
// Created by Göksu Güvendiren on 20/07/2018.
//

#pragma once

#include <Ray.hpp>

namespace gpt
{
    namespace shapes
    {
        class Shape;
    }

    namespace materials
    {
        class Material;
    }

    class HitInfo
    {
        float param;
        glm::vec3 normal;
        glm::vec3 position;
        Ray r;

        const gpt::shapes::Shape* shape;
    public:
        HitInfo(glm::vec3 n, glm::vec3 point, Ray ry, const gpt::shapes::Shape* hitShape, float t = std::numeric_limits<float>::infinity()) : param(t),
                                                                                                          normal(n),
                                                                                                          position(point),
                                                                                                          r(ry),
                                                                                                          shape(hitShape)
        {}

        float       Param() const { return param; }
        glm::vec3   Normal() const { return normal; }

        const gpt::shapes::Shape* Shape() const { return shape; }
        const gpt::materials::Material*  Material() const;
    };
}

