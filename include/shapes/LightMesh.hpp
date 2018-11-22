//
// Created by Göksu Güvendiren on 2018-11-21.
//

#pragma once

#include <materials/EmittingMaterial.hpp>
#include "shapes/Mesh.hpp"
#include "lights/Light.hpp"
#include "Triangle.hpp"

namespace gpt
{
    inline glm::vec3 SampleTriangle(const gpt::Triangle& triangle)
    {
        auto sqsample1 = std::sqrt(gpt::utils::sample_float());
        auto sample2   = gpt::utils::sample_float();

        glm::vec3 position = (1 - std::sqrt(sqsample1))  * triangle.PointA();
        position          += (sqsample1 * (1 - sample2)) * triangle.PointB();
        position          += (sqsample1 * sample2) * triangle.PointC();

        return position;
    }

//    namespace materials
//    {
//        class EmittingMaterial;
//    }

    class LightMesh : public shapes::Mesh, public Light
    {
    public:
        LightMesh(const gpt::materials::EmittingMaterial &m, std::vector<gpt::Triangle> &&fcs, int id) :
                            Mesh(m, std::move(fcs), id)
        {
            assert(m.Emitting() && "Material in a light mesh should be an emitting material.");
        }

        glm::vec3 Direction(const glm::vec3 &hitpos) const override
        {
            // choose a random triangle from the faces of the mesh
            auto index = (unsigned int) std::floor(gpt::utils::sample_float() * faces.size());
            // choose a random point from the chosen face of the mesh.
            auto position = SampleTriangle(faces[index]);

            return position - hitpos;
        }

        glm::vec3 Intensity(const glm::vec3& direction) const override
        {
            auto lensquared = [](const glm::vec3& n){return n.r * n.r + n.g * n.g + n.b * n.b;};
            return material->CalculateReflectance({}, {}, {}) / std::max(1.0f, lensquared(direction));
        }
    };
}
