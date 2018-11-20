//
// Created by Göksu Güvendiren on 20/07/2018.
//

#pragma once

#include "shapes/Shape.hpp"
#include <vector>
#include <tinyxml/tinyxml2.h>
#include <AABB.hpp>

namespace gpt
{
    class Scene;
    namespace shapes
    {
        class Sphere : public Shape
        {
            float radius;
            glm::vec3 center;

            glm::mat4 transformation_matrix;
            glm::mat4 inverse_transf;
            glm::mat4 inverse_transpose_transf;

        public:
            Sphere(int sid, float rd, glm::vec3 c, const gpt::Material& m) : radius(rd), center(c), Shape(m, c - rd, c + rd, sid)
            {
                transformation_matrix       = glm::mat4(1.0f);
                inverse_transf              = glm::mat4(1.0f);
                inverse_transpose_transf    = glm::mat4(1.0f);
            }

            void TransformationMatrix(const glm::mat4& mat)
            {
                transformation_matrix = mat;
                inverse_transf = glm::inverse(transformation_matrix);
                inverse_transpose_transf = glm::transpose(inverse_transf);
            }

            ~Sphere() override = default;

            boost::optional<HitInfo> Hit(const Ray &r) const override;
//            boost::optional<float>   ShadowHit(const Ray& ray) const;
        };
    }
}

