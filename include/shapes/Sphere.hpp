//
// Created by Göksu Güvendiren on 20/07/2018.
//

#ifndef RAYTRACER_SPHERE_HPP
#define RAYTRACER_SPHERE_HPP

#include "shapes/Shape.hpp"

namespace gpt
{
    namespace shapes
    {
        class Sphere : public Shape
        {
            int id;
            float radius;
            glm::vec3 center;

            glm::mat4 transformation_matrix;
            glm::mat4 inverse_transf;
            glm::mat4 inverse_transpose_transf;

        public:
            Sphere(int sid, float rd, glm::vec3 c) : id(sid), radius(rd), center(c)
            {
                transformation_matrix       = glm::mat4(1.0f);
                inverse_transf              = glm::mat4(1.0f);
                inverse_transpose_transf    = glm::mat4(1.0f);
            }

            ~Sphere() = default;

            boost::optional<HitInfo> Hit(const Ray &r) const;
//            boost::optional<float>   ShadowHit(const Ray& ray) const;
            int ID() const { return id; }

        };
    }
}

#endif //RAYTRACER_SPHERE_HPP
