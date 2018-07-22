//
// Created by Göksu Güvendiren on 21/07/2018.
//

#include <Scene.hpp>
#include <iostream>
#include <sstream>
#include <tinyxml/tinyxml2.h>
#include <Camera.hpp>

static glm::vec3 GetElem(tinyxml2::XMLElement* element)
{
    glm::vec3 color;

    std::istringstream ss {element->GetText()};
    ss >> color.r;
    ss >> color.g;
    ss >> color.b;

    return color;
}

boost::optional<gpt::HitInfo> gpt::Scene::Hit(const Ray &ray) const
{
    boost::optional<gpt::HitInfo> min_hit = boost::none;
    for (auto& shape : shapes)
    {
        auto hit = shape->Hit(ray);
        if (!hit) continue;
        if (!min_hit || hit->Param() < min_hit->Param())
        {
            min_hit = *hit;
        }
    }

    return min_hit;
}

void gpt::Scene::Load(const std::string& filename)
{
    tinyxml2::XMLDocument document;
    document.LoadFile(filename.c_str());

    if (document.Error()){
        document.PrintError();
        std::abort();
    }

    auto docscene = document.FirstChildElement("Scene");

    if (!docscene){
        std::cerr << "Not a valid scene file" << '\n';
        std::abort();
    }

    if (auto elem = docscene->FirstChildElement("BackgroundColor")){
        auto color = GetElem(elem);
        backgroundColor = color;
    }

    if (auto elem = docscene->FirstChildElement("ShadowRayEpsilon")){
        shadowRayEpsilon = elem->FloatText();
    }

//    if (auto elem = docscene->FirstChildElement("MaxRecursionDepth")){
//        maxrecdepth = (elem->IntText(1));
//    }

    if (auto elem = docscene->FirstChildElement("IntersectionTestEpsilon")){
        intersectionTestEpsilon = elem->FloatText();
    }

    gpt::Camera camera;
    if (auto elem = docscene->FirstChildElement("Camera")){
        camera = LoadCamera(elem);
    }
    else {
        std::cerr << "Could not read camera information\n";
        std::abort();
    }

    cameras.push_back(camera);
/*
    if (auto elem = docscene->FirstChildElement("Lights")){
        if (auto al = elem->FirstChildElement("AmbientLight")){
            auto ambient = GetElem(al);
            AmbientLight(ambient);
        }
        else
        {
            AmbientLight({0, 0, 0});
        }
        lights = LoadLights(elem);
    }

    if (auto elem = docscene->FirstChildElement("BRDFs"))
    {
        brdfs = LoadBRDFs(elem);
    }

    if (auto elem = docscene->FirstChildElement("Materials")){
        for (auto mat : LoadMaterials(elem))
        {
            AddMaterial(mat);
        }
    }

    if (auto elem = docscene->FirstChildElement("TexCoordData")){
        texCoords = LoadTexCoordData(elem);
    }
*/

//    if (auto elem = docscene->FirstChildElement("VertexData")){
//        vertices = LoadVertexData(elem, texCoords);
//    }
//
//    if (auto elem = docscene->FirstChildElement("Transformations")){
//        transformations  = LoadTransformations(elem);
//    }

/*
    if (auto elem = docscene->FirstChildElement("Textures"))
    {
        textures = LoadTextures(elem);
    }
*/
//
//    if(auto objects = docscene->FirstChildElement("Objects")){
//        triangles     = LoadTriangles(objects);
//        spheres       = LoadSpheres(objects);
//        meshes        = LoadMeshes(objects);
//        meshInstances = LoadMeshInstances(objects);
//    }

//    for (auto& mesh : meshes)
//    {
//        assert(mesh.GetMaterial());
//    }
/*

    AddCamera(camera);

    glm::vec3 mins, maxs;

    for_each(triangles.begin(), triangles.end(), [this, &mins, &maxs](auto& tri) {
        Compare(tri.Max(), mins, maxs);
        Compare(tri.Min(), mins, maxs);
        shapes.push_back(&tri);
    });

    for_each(meshes.begin(), meshes.end(), [this, &mins, &maxs](auto& msh) {
        Compare(msh.Max(), mins, maxs);
        Compare(msh.Min(), mins, maxs);

        shapes.push_back(&msh);
    });

    for_each(meshInstances.begin(), meshInstances.end(), [this, &mins, &maxs](auto& msh) {
        Compare(msh.Max(), mins, maxs);
        Compare(msh.Min(), mins, maxs);

        shapes.push_back(&msh);
    });

    if (shapes.size() > 0)
        boundingBox = BoundingVolume(shapes, Axis::X);
    else boundingBox = BoundingVolume();
     */
}