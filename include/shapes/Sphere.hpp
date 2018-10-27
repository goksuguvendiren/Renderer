//
// Created by Göksu Güvendiren on 20/07/2018.
//

#ifndef RAYTRACER_SPHERE_HPP
#define RAYTRACER_SPHERE_HPP

#include "shapes/Shape.hpp"
#include <vector>
#include <tinyxml/tinyxml2.h>

namespace gpt
{
    class Scene;
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
            Sphere(int sid, float rd, glm::vec3 c, const gpt::materials::Material* m) : id(sid), radius(rd), center(c), Shape(m)
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

            ~Sphere() = default;

            boost::optional<HitInfo> Hit(const Ray &r) const;
//            boost::optional<float>   ShadowHit(const Ray& ray) const;
            int ID() const { return id; }

            static std::vector<Sphere> Load(gpt::Scene& scene, tinyxml2::XMLElement *elem);
        };
    }
}

#endif //RAYTRACER_SPHERE_HPP
