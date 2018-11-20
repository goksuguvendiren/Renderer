//
// Created by Göksu Güvendiren on 2018-11-01.
//

#include <Scene.hpp>
#include <loaders/xml_loader.hpp>

#include <iostream>
#include <sstream>
#include <tinyxml/tinyxml2.h>

#include <camera.hpp>
#include <Transformation.hpp>
#include <materials/Material.hpp>
#include <lights/Light.hpp>
#include <materials/EmittingMaterial.hpp>
#include <materials/BasicMaterial.hpp>
#include <lights/PointLight.hpp>
#include "Utils.hpp"

using namespace gpt;

namespace
{
    std::unique_ptr<Material> LoadBasicMaterial(const MaterialLoadContext& context, tinyxml2::XMLElement *elem)
    {
        auto ambient = utils::GetElem(elem->FirstChildElement("AmbientReflectance"));
        auto diffuse = utils::GetElem(elem->FirstChildElement("DiffuseReflectance"));
        auto specular = utils::GetElem(elem->FirstChildElement("SpecularReflectance"));

        return std::make_unique<gpt::materials::BasicMaterial>(ambient, diffuse, specular);
    }

    std::unique_ptr<Material> LoadEmittingMaterial(const MaterialLoadContext& context, tinyxml2::XMLElement *elem)
    {
        auto tmp = elem->FirstChildElement("Radiance");

        if (!tmp)
        {
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

    boost::optional<gpt::shapes::Triangle> GetFace(const ShapeLoadContext& context, std::istringstream& stream, int vertexOffset, int texCoordsOffset,
                                                   const glm::mat4& matrix, int matID, int index, int texID = -1)
    {
        int x, y, z;

        if (!(stream >> x)) {
            return boost::none;
        }
        if (!(stream >> y)) {
            return boost::none;
        }
        if (!(stream >> z)) {
            return boost::none;
        }

        int a = x;
        int b = y;
        int c = z;

        x += vertexOffset;
        y += vertexOffset;
        z += vertexOffset;

        auto ind0 = glm::vec4(context.GetVertex(x), 1);
        auto ind1 = glm::vec4(context.GetVertex(y), 1);
        auto ind2 = glm::vec4(context.GetVertex(z), 1);

        ind0 = matrix * ind0;
        ind1 = matrix * ind1;
        ind2 = matrix * ind2;

        return gpt::shapes::Triangle(index,
                                     {ind0.x, ind0.y, ind0.z},
                                     {ind1.x, ind1.y, ind1.z},
                                     {ind2.x, ind2.y, ind2.z},
                                     context.GetMaterial(matID));
    }

    std::vector<glm::vec3> LoadVertexData(tinyxml2::XMLElement *elem)
    {
        std::istringstream stream { elem->GetText() };

        std::vector<glm::vec3> verts;
        while(stream)
        {
            verts.push_back(gpt::utils::GetVertex(stream));
        }

        return verts;
    }

    std::map<int, std::unique_ptr<Material>> LoadMaterials(const MaterialLoadContext& scene, tinyxml2::XMLElement *elem)
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

    std::vector<gpt::shapes::Triangle> LoadTriangle(const ShapeLoadContext& context, tinyxml2::XMLElement* elem)
    {
        std::vector<gpt::shapes::Triangle> tris;

        for (auto child = elem->FirstChildElement("Triangle"); child != nullptr; child = child->NextSiblingElement("Triangle")) {
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
                transformations = gpt::utils::GetTransformations(ss);
            }

            auto id0 = utils::GetInt(child->FirstChildElement("Indices"));
            auto id1 = utils::GetInt(child->FirstChildElement("Indices"));
            auto id2 = utils::GetInt(child->FirstChildElement("Indices"));

            auto ind0 = glm::vec4(context.GetVertex(id0), 1);
            auto ind1 = glm::vec4(context.GetVertex(id1), 1);
            auto ind2 = glm::vec4(context.GetVertex(id2), 1);

            glm::mat4 matrix(1.0f);
            for (auto& tr : transformations)
            {
                auto m = context.GetTransformation(tr);
                matrix = m * matrix;
            }

            ind0 = matrix * ind0;
            ind1 = matrix * ind1;
            ind2 = matrix * ind2;

            tris.emplace_back(gpt::shapes::Triangle(id,
                                                    {ind0.x, ind0.y, ind0.z},
                                                    {ind1.x, ind1.y, ind1.z},
                                                    {ind2.x, ind2.y, ind2.z},
                                                    context.GetMaterial(matID)));
        }

        return tris;
    }

    std::vector<gpt::shapes::Sphere> LoadSphere(const ShapeLoadContext& context, tinyxml2::XMLElement *elem)
    {
        std::vector<gpt::shapes::Sphere> spheres;

        for (auto child = elem->FirstChildElement("Sphere"); child != nullptr; child = child->NextSiblingElement("Sphere")) {
            int id;
            child->QueryIntAttribute("id", &id);
            int materialID  = child->FirstChildElement("Material")->IntText(0);
            int centerID    = child->FirstChildElement("Center")->IntText(0);
            float radius    = child->FirstChildElement("Radius")->FloatText(0);

            glm::vec3 center = context.GetVertex(centerID);

            std::vector<std::string> transformations;
            if(auto trns = child->FirstChildElement("Transformations"))
            {
                transformations = gpt::utils::GetTransformations(trns);
            }

            glm::mat4 matrix(1.0f);
            for (auto& tr : transformations)
            {
                auto m = context.GetTransformation(tr);
                matrix = m * matrix;
            }

            int tid = -1;
            if (auto tex = child->FirstChildElement("Texture"))
            {
                tid = tex->IntText(-1);
            }

            gpt::shapes::Sphere sp {id, radius, center, context.GetMaterial(materialID)};
            sp.TransformationMatrix(matrix);

            spheres.push_back(std::move(sp));
        }

        return spheres;
    }

    std::vector<gpt::shapes::Mesh> LoadMesh(const ShapeLoadContext& context, tinyxml2::XMLElement *elem)
    {
        std::vector<gpt::shapes::Mesh> meshes;

        for (auto child = elem->FirstChildElement("Mesh"); child != nullptr; child = child->NextSiblingElement("Mesh"))
        {
            int id;
            child->QueryIntAttribute("id", &id);

            bool is_art = false;
            if (child->QueryBoolText(&is_art))
            {
                is_art = true;
            }

            int matID = child->FirstChildElement("Material")->IntText(0);
            int texID = -1;
            if (auto sth = child->FirstChildElement("Texture")){
                texID = sth->IntText(0);
            }

            std::vector<std::string> transformations;
            if(auto trns = child->FirstChildElement("Transformations"))
            {
                std::istringstream ss {trns->GetText()};
                transformations = gpt::utils::GetTransformations(ss);
            }

            glm::mat4 matrix(1.0f);
            for (auto& tr : transformations){
                auto m = context.GetTransformation(tr);
                matrix = m * matrix;
            }

            auto FaceData = child->FirstChildElement("Faces");
            std::istringstream stream { FaceData->GetText() };
            int vertexOffset = 0;
            int texCoordOffset = 0;

            if (FaceData->QueryIntAttribute("vertexOffset", &vertexOffset))
                ;
            if (FaceData->QueryIntAttribute("textureOffset", &texCoordOffset))
                ;

            boost::optional<gpt::shapes::Triangle> tr;
            std::vector<gpt::shapes::Triangle> faces;

            while((tr = GetFace(context, stream, vertexOffset, texCoordOffset, matrix, matID, id, texID)))
            {
                auto tri = *tr;
                faces.push_back(std::move(*tr));
            }

            gpt::shapes::Mesh msh{context.GetMaterial(matID), std::move(faces), id};
            meshes.push_back(std::move(msh));
        }

        return meshes;
    }

    std::vector<std::unique_ptr<gpt::Light>> LoadLight(tinyxml2::XMLElement *elem)
    {
        std::vector<std::unique_ptr<gpt::Light>> lights;
        for (auto child = elem->FirstChildElement("PointLight"); child != nullptr; child = child->NextSiblingElement("PointLight"))
        {
            int id;
            child->QueryIntAttribute("id", &id);

            auto position = utils::GetElem(child->FirstChildElement("Position"));
            auto intensity = utils::GetElem(child->FirstChildElement("Intensity"));

            lights.push_back(std::make_unique<gpt::lights::PointLight>(position, intensity));
        }
        return lights;
    }

    gpt::Camera LoadCamera(tinyxml2::XMLElement *element)
    {
        int id;
        if (element->QueryIntAttribute("id", &id) == tinyxml2::XML_NO_ATTRIBUTE)
        {
            std::cerr << "No such attribute as id" << '\n';
            std::abort();
        }

        glm::vec3 position = utils::GetElem(element->FirstChildElement("Position"));
        glm::vec3 gaze = utils::GetElem(element->FirstChildElement("Gaze"));
        glm::vec3 up = utils::GetElem(element->FirstChildElement("Up"));

        tinyxml2::XMLElement* elem;
        int sampleCount = 1;

        if ((elem = element->FirstChildElement("NumSamples")))
        {
            sampleCount = elem->IntText(1);
        }

        int focalDistance = 1;
        if ((elem = element->FirstChildElement("FocusDistance"))){
            focalDistance = elem->IntText(1);
        }

        float apertureSize = 0;
        if ((elem = element->FirstChildElement("ApertureSize"))){
            apertureSize = elem->FloatText(0);
        }

        gpt::ImagePlane plane = gpt::CreatePlane(element, focalDistance);

        std::string name = element->FirstChildElement("ImageName")->GetText();

        return gpt::Camera(plane, id, position, gaze, up, name, sampleCount, focalDistance, apertureSize);
    }
}

gpt::Scene load_scene(const std::string& filename)
{
    SceneMeta meta;
    MaterialLoadContext materialContext;

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
        meta.backgroundColor = color;
    }

    if (auto elem = docscene->FirstChildElement("ShadowRayEpsilon"))
    {
        meta.shadowRayEpsilon = elem->FloatText();
    }

    if (auto elem = docscene->FirstChildElement("MaxRecursionDepth"))
    {
        meta.maxRecursionDepth = (elem->IntText(1));
    }

    if (auto elem = docscene->FirstChildElement("IntersectionTestEpsilon"))
    {
        meta.intersectionTestEpsilon = elem->FloatText();
    }

    if (auto elem = docscene->FirstChildElement("AmbientLight"))
    {
        auto color = utils::GetElem(elem);
        meta.ambientLight = color;
    }

    gpt::Camera camera;
    if (auto elem = docscene->FirstChildElement("Camera"))
    {
        camera = std::move(LoadCamera(elem));
    }
    else
        {
        std::cerr << "Could not read camera information\n";
        std::abort();
    }

    meta.cameras.push_back(camera);
    if (auto elem = docscene->FirstChildElement("Lights"))
    {
        meta.lights = LoadLight(elem);
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
        meta.vertices = LoadVertexData(elem);
    }

    if (auto elem = docscene->FirstChildElement("Transformations"))
    {
        meta.transformations  = gpt::LoadTransformations(elem);
    }

    if (auto elem = docscene->FirstChildElement("Materials"))
    {
        meta.materials = LoadMaterials(materialContext, elem);
    }

    if(auto objects = docscene->FirstChildElement("Objects"))
    {
        ShapeLoadContext shapeContext(meta.vertices, meta.transformations, meta.materials);
        meta.triangles     = LoadTriangle(shapeContext, objects);
        meta.spheres       = LoadSphere(shapeContext, objects);
        meta.meshes        = std::move(LoadMesh(shapeContext, objects));

        for (auto& tri : meta.triangles) meta.shapes.push_back(&tri);
        for (auto& sph : meta.spheres)   meta.shapes.push_back(&sph);
        for (auto& msh : meta.meshes)    meta.shapes.push_back(&msh);
    }

    meta.aabb = gpt::AABB(meta.shapes);

    return Scene(std::move(meta));
}