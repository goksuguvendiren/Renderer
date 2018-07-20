#include <iostream>
#include <shapes/Sphere.hpp>
#include <shapes/Triangle.hpp>

int main()
{
    gpt::shapes::Sphere sphere(0, 1, {0, 0, 0});

    gpt::shapes::Triangle triangle(0, {-1, 1, 0}, {1,1, 0}, {0, 2, 0});
    gpt::Ray ray({0, 0, -5}, {0, 0, 1}, true);

    if (triangle.Hit(ray))
    {
        std::cerr << "yey" << '\n';
    }
    else
    {
        std::cerr << "nay" << '\n';
    }
    return 0;
}