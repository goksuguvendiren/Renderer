//
// Created by Göksu Güvendiren on 2018-11-19.
//

#include <shapes/Shape.hpp>
#include <shapes/Triangle.hpp>
#include <iostream>

gpt::Shape::Shape(const gpt::Material& m, const std::vector<gpt::shapes::Triangle>& faces, int id) : material(&m), id(id)
{
    aabb = gpt::AABB(faces);
}