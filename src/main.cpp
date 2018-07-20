#include <iostream>
#include <shapes/Sphere.hpp>

int main()
{
    gpt::shapes::Sphere sphere(0, 1, {0, 0, 0});
    gpt::Ray ray({0, 0, -5}, {0, 0, 1}, true);

    if (sphere.Hit(ray))
    {
        std::cerr << "yey" << '\n';
    }
    else
    {
        std::cerr << "nay" << '\n';
    }
    return 0;
}