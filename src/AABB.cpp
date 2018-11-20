//
// Created by Göksu Güvendiren on 2018-11-19.
//

#include <AABB.hpp>
#include <shapes/Shape.hpp>
#include <shapes/Triangle.hpp>

#include <iostream>

gpt::AABB::AABB(Shape* newshape, const glm::vec3& mins, const glm::vec3& maxs)
{
    shape = newshape;
    box = gpt::Box(mins, maxs);
}

gpt::AABB::AABB(const std::vector<gpt::Shape*> &shapes, Axis axis) : left(nullptr), right(nullptr), shape(nullptr)
{
    num_shapes = shapes.size();
    if (shapes.size() == 1)
    {
        shape = shapes[0];
        box   = gpt::Box(shapes[0]->Min(), shapes[0]->Max());

        return;
    }

    std::vector<Shape*> leftshapes;  leftshapes.reserve(shapes.size() / 2 + 1);
    std::vector<Shape*> rightshapes; rightshapes.reserve(shapes.size() / 2 + 1);

    auto sortedShapes = shapes;

    Axis nextAxis;
    switch(axis)
    {
        case Axis::X :
            std::sort(sortedShapes.begin(), sortedShapes.end(), [](auto& sh1, auto& sh2)
            {
                return sh1->Middle().x < sh2->Middle().x;
            });

            nextAxis = Axis::Y;
            break;

        case Axis::Y :
            std::sort(sortedShapes.begin(), sortedShapes.end(), [](auto& sh1, auto& sh2)
            {
                return sh1->Middle().y < sh2->Middle().y;
            });

            nextAxis = Axis::Z;
            break;

        case Axis::Z :
            std::sort(sortedShapes.begin(), sortedShapes.end(), [](auto& sh1, auto& sh2)
            {
                return sh1->Middle().z < sh2->Middle().z;
            });

            nextAxis = Axis::X;
            break;
    }

    auto beginning = sortedShapes.begin();
    auto middling  = sortedShapes.begin() + (sortedShapes.size() / 2);
    auto ending    = sortedShapes.end();

    leftshapes  = std::vector<Shape*>(beginning, middling);
    rightshapes = std::vector<Shape*>(middling, ending);

    assert(shapes.size() == (leftshapes.size() + rightshapes.size()));

    left  = new AABB(leftshapes, nextAxis);
    right = new AABB(rightshapes, nextAxis);

    box.Min(glm::min(left->box.Min(), right->box.Min()));
    box.Max(glm::max(left->box.Max(), right->box.Max()));
}

void gpt::AABB::Print(int depth) const
{
    for (int i = 0; i < depth; ++i)
    {
        std::cerr << '\t';
    }
    std::cerr << "num_shapes : " << num_shapes << '\n';
    for (int i = 0; i < depth; ++i)
    {
        std::cerr << '\t';
    }
    std::cerr << "mins       : " << box.Min().x << ", " << box.Min().y << ", " << box.Min().z << "\n";
    for (int i = 0; i < depth; ++i)
    {
        std::cerr << '\t';
    }
    std::cerr << "maxs       : " << box.Max().x << ", " << box.Max().y << ", " << box.Max().z << "\n";

    if (left)
    {
        left->Print(depth + 1);
    }
    if (right)
    {
        right->Print(depth + 1);
    }
}

bool gpt::Box::Hit(const gpt::Ray& ray) const
{
    auto minvals = (mins - ray.Origin()) * ray.InvDirection();
    auto maxvals = (maxs- ray.Origin()) * ray.InvDirection();

    float tx1 = minvals.x;
    float tx2 = maxvals.x;

    float tmin = std::min(tx1, tx2);
    float tmax = std::max(tx1, tx2);

    float ty1 = minvals.y;
    float ty2 = maxvals.y;

    tmin = std::max(tmin, std::min(ty1, ty2));
    tmax = std::min(tmax, std::max(ty1, ty2));

    float tz1 = minvals.z;
    float tz2 = maxvals.z;

    tmin = std::max(tmin, std::min(tz1, tz2));
    tmax = std::min(tmax, std::max(tz1, tz2));

    return tmax >= std::max(0.f, tmin);
}

boost::optional<gpt::HitInfo> gpt::AABB::Hit(const gpt::Ray& ray) const
{
    if (!box.Hit(ray))   return boost::none;
    if (!left && !right) return shape->Hit(ray);

    boost::optional<HitInfo> leftHitInfo  = left  ? left->Hit(ray)  : boost::none;
    boost::optional<HitInfo> rightHitInfo = right ? right->Hit(ray) : boost::none;

    if (!leftHitInfo && !rightHitInfo) return boost::none;

    HitInfo ultimate;

    if (leftHitInfo)
    {
        ultimate = *leftHitInfo;
    }
    if (rightHitInfo && rightHitInfo->Param() < ultimate.Param())
    {
        ultimate = *rightHitInfo;
    }

    return ultimate;
}

void gpt::Box::Update(const glm::vec3 &val)
{
    mins = glm::min(mins, val);
    maxs = glm::max(maxs, val);
}

std::vector<gpt::Shape*> gpt::AABB::to_ptrs(const std::vector<gpt::shapes::Triangle> &triangles)
{
    std::vector<gpt::Shape*> shapes(triangles.size());

    for(int i = 0; i < triangles.size(); i++)
    {
        const gpt::Shape* s = triangles.data() + i;
        shapes[i] = const_cast<gpt::Shape*>(s);
    }

    return shapes;
}
