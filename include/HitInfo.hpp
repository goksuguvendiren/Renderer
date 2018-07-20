//
// Created by Göksu Güvendiren on 20/07/2018.
//

#ifndef RAYTRACER_HITINFO_HPP
#define RAYTRACER_HITINFO_HPP

#include "Ray.hpp"

namespace gpt
{
    class HitInfo
    {
        float param;
        glm::vec3 normal;
        glm::vec3 position;
        Ray r;

    public:
        HitInfo(glm::vec3 n, glm::vec3 point, Ray ry, float t = std::numeric_limits<float>::infinity()) : param(t),
                                                                                                          normal(n),
                                                                                                          position(point),
                                                                                                          r(ry)
        {}
    };
}

#endif //RAYTRACER_HITINFO_HPP
