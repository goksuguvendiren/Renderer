//
// Created by Göksu Güvendiren on 21/07/2018.
//

#include <shapes/Triangle.hpp>
#include <shapes/Mesh.hpp>
#include "catch2/catch.hpp"
#include "shapes/Sphere.hpp"

TEST_CASE( "Sphere hit is tested", "[sphere_hit]" )
{
    gpt::shapes::Sphere sphere(0, 1, {0, 0, 0});
    gpt::Ray ray({0, 0, -5}, {0, 0, 1}, true);

    auto req = sphere.Hit(ray) != boost::none;

    REQUIRE(req);
}

TEST_CASE( "Triangle hit is tested", "[triangle_hit]" )
{
    gpt::shapes::Triangle triangle1(0, {-1, 1, 0}, {1, 1, 0}, {0, 2, 0});
    gpt::shapes::Triangle triangle2(0, {-1, 2, 0}, {1, 2, 0}, {0, 3, 0});
    gpt::shapes::Triangle triangle3(0, {-1, 0, 0}, {1, 0, 0}, {0, 1, 0});

    gpt::Ray ray({0, 0, -5}, {0, 0, 1}, true);

    auto req = triangle1.Hit(ray) == boost::none;
    REQUIRE(req);
    req = triangle2.Hit(ray) == boost::none;
    REQUIRE(req);
    req = triangle3.Hit(ray) != boost::none;
    REQUIRE(req);
}

TEST_CASE( "Mesh hit is tested", "[mesh_hit]" )
{
    gpt::shapes::Triangle triangle1(0, {-1, 1, 0}, {1, 1, 0}, {0, 2, 0});
    gpt::shapes::Triangle triangle2(0, {-1, 2, 0}, {1, 2, 0}, {0, 3, 0});
    gpt::shapes::Triangle triangle3(0, {-1, 0, 0}, {1, 0, 0}, {0, 1, 0});

    gpt::shapes::Mesh m(1);

    m.AddFace(std::move(triangle1));
    m.AddFace(std::move(triangle2));

    gpt::Ray ray({0, 0, -5}, {0, 0, 1}, true);

    auto req = m.Hit(ray) == boost::none;
    REQUIRE(req);

    m.AddFace(std::move(triangle3));
    req = m.Hit(ray) != boost::none;
    REQUIRE(req);
}