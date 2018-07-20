//
// Created by Göksu Güvendiren on 20/07/2018.
//

#ifndef RAYTRACER_RAY_HPP
#define RAYTRACER_RAY_HPP

#include <glm/glm.hpp>

namespace gpt
{
    class Ray
    {
        glm::vec3   origin;
        glm::vec3   direction;
        glm::vec3   invdirection;

        bool        is_primary;

    public:
        Ray() : Ray({0, 0, 0}, {1, 0, 0}) {};
        Ray(glm::vec3 o, glm::vec3 d, bool isp = true) : origin(o), direction(d), is_primary(isp)
        {
            invdirection = glm::vec3(1.f, 1.f, 1.f) / direction;
        };

        glm::vec3 Origin() const { return origin; }
        glm::vec3 Direction() const { return direction; }
        glm::vec3 InvDirection() const { return invdirection; }

        bool IsPrimary() const { return is_primary; }
    };
}

#endif //RAYTRACER_RAY_HPP
