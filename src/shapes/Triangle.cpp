//
// Created by Göksu Güvendiren on 20/07/2018.
//

#include <sstream>
#include "shapes/Triangle.hpp"
#include "Scene.hpp"

inline float determinant(const glm::vec3& col1,
                         const glm::vec3& col2,
                         const glm::vec3& col3)
{
    return col1.x * (col2.y * col3.z - col2.z * col3.y) -
           col2.x * (col1.y * col3.z - col1.z * col3.y) +
           col3.x * (col1.y * col2.z - col1.z * col2.y);

}

boost::optional<gpt::HitInfo> gpt::shapes::Triangle::Hit (const gpt::Ray& ray) const
{
    glm::vec3 col1 = pointA - pointB;
    glm::vec3 col2 = pointA - pointC;
    glm::vec3 col3 = ray.Direction();
    glm::vec3 col4 = pointA - ray.Origin();

    auto detA  = determinant(col1, col2, col3);
    if (detA == 0) return boost::none;

    auto beta  = determinant(col4, col2, col3) / detA;
    auto gamma = determinant(col1, col4, col3) / detA;
    auto param = determinant(col1, col2, col4) / detA;
    auto alpha = 1 - beta - gamma;

    if (alpha < -0.0001 || gamma < -0.0001 || beta < -0.0001 || param < -0.0001) return boost::none;

    auto point = ray.Origin() + param * ray.Direction();

    glm::vec3 normal = glm::normalize(alpha * surfNormal + beta * surfNormal + gamma * surfNormal);

    return gpt::HitInfo(normal, point, ray, param);
}

gpt::shapes::Triangle::Triangle(int i, glm::vec3 a, glm::vec3 b, glm::vec3 c, int tid, int tr_id)
{
    id = i;
    pointA = a;
    pointB = b;
    pointC = c;

    surfNormal = glm::normalize(glm::cross(pointB - pointA, pointC - pointA));
}

int GetInt(std::istringstream& stream)
{
    int val;
    stream >> val;
    return val;
}

std::vector<gpt::shapes::Triangle> gpt::shapes::Triangle::Load(gpt::Scene& scene, tinyxml2::XMLElement* elem)
{
    std::vector<Triangle> tris;

    for (auto child = elem->FirstChildElement("Triangle"); child != NULL; child = child->NextSiblingElement("Triangle")) {
        int id;
        child->QueryIntAttribute("id", &id);
        int matID = child->FirstChildElement("Material")->IntText(0);

        int texID = -1;
        if (auto texelem = child->FirstChildElement("Texture"))
        {
            texID = texelem->IntText(-1);
        }

        std::vector<std::string> transformations;
        if(auto trns = child->FirstChildElement("Transformations")){
            std::istringstream ss {trns->GetText()};
            transformations = gpt::GetTransformations(ss);
        }

        std::istringstream stream {child->FirstChildElement("Indices")->GetText()};

        auto id0 = GetInt(stream);
        auto id1 = GetInt(stream);
        auto id2 = GetInt(stream);

        auto ind0 = glm::vec4(scene.GetVertex(id0), 1);
        auto ind1 = glm::vec4(scene.GetVertex(id1), 1);
        auto ind2 = glm::vec4(scene.GetVertex(id2), 1);

        glm::mat4 matrix(1.0f);
        for (auto& tr : transformations)
        {
            auto m = scene.GetTransformation(tr);
            matrix = m * matrix;
        }

        ind0 = matrix * ind0;
        ind1 = matrix * ind1;
        ind2 = matrix * ind2;

//        if (texID != -1 && !scene.GetTexture(texID).IsPerlin())
//        {
            tris.emplace_back(gpt::shapes::Triangle(id,
                              {ind0.x, ind0.y, ind0.z},
                              {ind1.x, ind1.y, ind1.z},
                              {ind2.x, ind2.y, ind2.z}));
//        }
//        else
//        {
//            tris.push_back({Vertex{id0, {ind0.x, ind0.y, ind0.z}},
//                            Vertex{id1, {ind1.x, ind1.y, ind1.z}},
//                            Vertex{id2, {ind2.x, ind2.y, ind2.z}},
//                            scene.GetMaterial(matID), texID, id});
//        }

    }

    return tris;
}