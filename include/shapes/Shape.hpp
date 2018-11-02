//
// Created by Göksu Güvendiren on 20/07/2018.
//

#pragma once

#include <glm/glm.hpp>
#include <boost/optional.hpp>
#include "HitInfo.hpp"
#include <sstream>
#include <vector>

namespace gpt
{
    inline auto GetTransformations(std::istringstream& stream)
    {
        std::vector<std::string> result;

        while(stream.good())
        {
            std::string tr;
            stream >> tr;
            result.push_back(tr);
        }

        return result;
    }

    class Ray;

    class Material;

    namespace shapes
    {
        class Shape
        {
            const gpt::Material* material;

        public:
            explicit Shape(const gpt::Material& m) : material(&m) {}
            virtual ~Shape() = default;
            virtual boost::optional<HitInfo> Hit(const Ray &ray) const = 0;
//            virtual boost::optional<float> ShadowHit(const Ray &ray) const = 0;

            const gpt::Material& Material() const { return *material; }
//            virtual bool isArtificial() const = 0;

//            virtual glm::vec3 Min() const = 0;
//            virtual glm::vec3 Max() const = 0;
//            virtual glm::vec3 Middle() const = 0;
        };
    }
}

