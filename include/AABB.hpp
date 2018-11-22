//
// Created by Göksu Güvendiren on 2018-11-19.
//

#pragma once

#include <boost/optional/optional.hpp>
#include <glm/vec3.hpp>
#include <algorithm>
#include <vector>
#include <Ray.hpp>

#include <HitInfo.hpp>
#include <shapes/Triangle.hpp>

namespace gpt
{
    class Box
    {
        float inf = std::numeric_limits<float>::infinity();
        glm::vec3 mins = {inf, inf, inf};
        glm::vec3 maxs = {-inf, -inf, -inf};
        glm::vec3 midd = {0, 0, 0};

    public:
        Box() = default;
        Box(const glm::vec3& minval, const glm::vec3& maxval) : mins(minval), maxs(maxval)
        {
            midd = (mins + maxs) / 2.0f;
        }

        bool Hit(const gpt::Ray &ray) const;

        void Min(glm::vec3 val) { mins = val; }
        void Max(glm::vec3 val) { maxs = val; }

        const glm::vec3& Min()    const { return mins; }
        const glm::vec3& Max()    const { return maxs; }
        const glm::vec3& Middle() const { return midd; }

        void Update(const glm::vec3 &val);

    };

    class Shape;
    namespace shapes
    {
        class Triangle;
    }

    class AABB
    {
        unsigned long num_triangles;
        Box box;

        AABB* left;
        AABB* right;
        const gpt::shapes::Triangle* triangle;

        enum class Axis { X, Y, Z};

    public:
        AABB() : num_triangles(), left(nullptr), right(nullptr), triangle(nullptr){}
        AABB(const std::vector<gpt::shapes::Triangle*>& triangles, Axis axis = Axis::X);

        const glm::vec3& Min()    const { return box.Min(); }
        const glm::vec3& Max()    const { return box.Max(); }
        const glm::vec3& Middle() const { return box.Middle(); }

        void Print(int depth = 0) const;

        boost::optional<gpt::HitInfo> Hit (const gpt::Ray& ray) const;

    };
}