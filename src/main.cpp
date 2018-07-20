#include <iostream>
#include <shapes/Sphere.hpp>
#include <shapes/Triangle.hpp>
#include <shapes/Mesh.hpp>

int main()
{
    gpt::shapes::Sphere sphere(0, 1, {0, 0, 0});

    gpt::shapes::Triangle triangle1(0, {-1, 1, 0}, {1, 1, 0}, {0, 2, 0});
    gpt::shapes::Triangle triangle2(0, {-1, 2, 0}, {1, 2, 0}, {0, 3, 0});
    gpt::shapes::Triangle triangle3(0, {-1, 0, 0}, {1, 0, 0}, {0, 1, 0});
    gpt::shapes::Mesh m(1);

    m.AddFace(std::move(triangle1));
    m.AddFace(std::move(triangle1));

    gpt::Ray ray({0, 0, -5}, {0, 0, 1}, true);

    if (m.Hit(ray))
    {
        std::cerr << "yey" << '\n';
    }
    else
    {
        std::cerr << "nay" << '\n';
    }
    return 0;
}