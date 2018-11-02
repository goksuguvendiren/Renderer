//
// Created by Göksu Güvendiren on 2018-11-01.
//

#include <Scene.hpp>

#include <iostream>
#include <sstream>
#include <tinyxml/tinyxml2.h>

#include <Camera.hpp>
#include <Transformation.hpp>
#include <materials/Material.hpp>
#include <lights/Light.hpp>
#include <materials/EmittingMaterial.hpp>
#include <materials/BasicMaterial.hpp>
#include "Utils.hpp"

using namespace gpt;

Scene load_scene(const std::string& file);

namespace
{
    std::unique_ptr<Material> LoadBasicMaterial(const gpt::Scene &scene, tinyxml2::XMLElement *elem)
    {
        auto ambient = utils::GetElem(elem->FirstChildElement("AmbientReflectance"));
        auto diffuse = utils::GetElem(elem->FirstChildElement("DiffuseReflectance"));
        auto specular = utils::GetElem(elem->FirstChildElement("SpecularReflectance"));

        return std::make_unique<gpt::materials::BasicMaterial>(ambient, diffuse, specular);
    }

    std::unique_ptr<Material> LoadEmittingMaterial(const gpt::Scene &scene, tinyxml2::XMLElement *elem)
    {
        auto tmp = elem->FirstChildElement("Radiance");

        if (!tmp) {
            throw std::runtime_error("Not an emitting material");
        }

        auto radiance = utils::GetElem(tmp);
        return std::make_unique<gpt::materials::EmittingMaterial>(radiance);
    }

    std::map<std::string, MaterialFactory> loaders = {
            {"Basic", LoadBasicMaterial },
            {"Emitting", LoadEmittingMaterial },
            {"BRDFMat", LoadBasicMaterial },
            {"Mirror", LoadBasicMaterial },
            {"Glass", LoadBasicMaterial }
    };

    static glm::vec3 GetVertex(std::istringstream& stream)
    {
        glm::vec3 vert;

        float datax;
        float datay;
        float dataz;

        stream >> datax;
        stream >> datay;
        stream >> dataz;

        return glm::vec3{datax, datay, dataz};
    }

    std::vector<glm::vec3> LoadVertexData(tinyxml2::XMLElement *elem)
    {
        std::istringstream stream { elem->GetText() };

        std::vector<glm::vec3> verts;
        while(stream)
        {
            verts.push_back(GetVertex(stream));
        }

        return verts;
    }

    std::map<int, std::unique_ptr<Material>> LoadMaterials(const Scene& scene, tinyxml2::XMLElement *elem)
    {
        std::map<int, std::unique_ptr<Material>> result;
        for (auto child = elem->FirstChildElement("Material"); child != nullptr; child = child->NextSiblingElement())
        {
            int id;
            child->QueryIntAttribute("id", &id);
            std::string type = child->Attribute("Type");

            if (loaders.find(type) == loaders.end())
            {
                throw std::runtime_error("no such material");
            }

            auto material = loaders[type](scene, child);
            result.emplace(id, std::move(material));
        }

        return result;
    }
}

void gpt::Scene::Load(const std::string& filename)
{
    tinyxml2::XMLDocument document;
    document.LoadFile(filename.c_str());

    if (document.Error())
    {
        document.PrintError();
        std::abort();
    }

    auto docscene = document.FirstChildElement("Scene");

    if (!docscene){
        std::cerr << "Not a valid scene file" << '\n';
        std::abort();
    }

    if (auto elem = docscene->FirstChildElement("BackgroundColor"))
    {
        auto color = utils::GetElem(elem);
        backgroundColor = color;
    }

    if (auto elem = docscene->FirstChildElement("ShadowRayEpsilon"))
    {
        shadowRayEpsilon = elem->FloatText();
    }

    if (auto elem = docscene->FirstChildElement("MaxRecursionDepth"))
    {
        maxRecursionDepth = (elem->IntText(1));
    }

    if (auto elem = docscene->FirstChildElement("IntersectionTestEpsilon"))
    {
        intersectionTestEpsilon = elem->FloatText();
    }

    if (auto elem = docscene->FirstChildElement("AmbientLight"))
    {
        auto color = utils::GetElem(elem);
        ambientLight = color;
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
    if (auto elem = docscene->FirstChildElement("Lights"))
    {
        lights = gpt::lights::Light::Load(*this, elem);
    }
/*

    if (auto elem = docscene->FirstChildElement("BRDFs"))
    {
        brdfs = LoadBRDFs(elem);
    }

    if (auto elem = docscene->FirstChildElement("TexCoordData")){
        texCoords = LoadTexCoordData(elem);
    }
*/

    if (auto elem = docscene->FirstChildElement("VertexData"))
    {
        vertices = LoadVertexData(elem);
    }

    if (auto elem = docscene->FirstChildElement("Transformations"))
    {
        transformations  = gpt::LoadTransformations(elem);
    }

/*
    if (auto elem = docscene->FirstChildElement("Textures"))
    {
        textures = LoadTextures(elem);
    }
*/

    if (auto elem = docscene->FirstChildElement("Materials"))
    {
        materials = LoadMaterials(*this, elem);
    }

    if(auto objects = docscene->FirstChildElement("Objects"))
    {
        triangles     = gpt::shapes::Triangle::Load(*this, objects);
        spheres       = gpt::shapes::Sphere::Load(*this, objects);
        meshes        = gpt::shapes::Mesh::Load(*this, objects);

        for (auto& tri : triangles) shapes.push_back(&tri);
        for (auto& sph : spheres)   shapes.push_back(&sph);
        for (auto& msh : meshes)    shapes.push_back(&msh);
    }

//    for (auto& mesh : meshes)
//    {
//        assert(mesh.GetMaterial());
//    }
//
//    glm::vec3 mins, maxs;
//
//    std::for_each(triangles.begin(), triangles.end(), [this, &mins, &maxs](auto& tri)
//    {
//        Compare(tri.Max(), mins, maxs);
//        Compare(tri.Min(), mins, maxs);
//        shapes.push_back(&tri);
//    });
//
//    std::for_each(spheres.begin(), spheres.end(), [this, &mins, &maxs](auto& sph)
//    {
//        Compare(tri.Max(), mins, maxs);
//        Compare(tri.Min(), mins, maxs);
//        shapes.push_back(&sph);
//    });
//
//    std::for_each(meshes.begin(), meshes.end(), [this, &mins, &maxs](auto& msh) {
//        Compare(msh.Max(), mins, maxs);
//        Compare(msh.Min(), mins, maxs);
//
//        shapes.push_back(&msh);
//    });
//
//    std::for_each(meshInstances.begin(), meshInstances.end(), [this, &mins, &maxs](auto& msh) {
//        Compare(msh.Max(), mins, maxs);
//        Compare(msh.Min(), mins, maxs);
//
//        shapes.push_back(&msh);
//    });
//
//    if (shapes.size() > 0)
//        boundingBox = BoundingVolume(shapes, Axis::X);
//    else boundingBox = BoundingVolume();

}