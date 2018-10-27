//
// Created by Göksu Güvendiren on 21/07/2018.
//

#include <shapes/Mesh.hpp>
#include "Scene.hpp"

boost::optional<gpt::HitInfo> gpt::shapes::Mesh::Hit(const Ray &ray) const
{
    boost::optional<gpt::HitInfo> min_hit = boost::none;
    for (auto& face : faces)
    {
        auto hit = face.Hit(ray);
        if (!hit) continue;
        if (!min_hit || hit->Param() < min_hit->Param())
        {
            min_hit = *hit;
        }
    }

    return min_hit;
}

boost::optional<gpt::shapes::Triangle> GetFace(gpt::Scene& scene, std::istringstream& stream, int vertexOffset, int texCoordsOffset,
                                         const glm::mat4& matrix, int matID, int index, int texID)
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

    auto ind0 = glm::vec4(scene.GetVertex(x), 1);
    auto ind1 = glm::vec4(scene.GetVertex(y), 1);
    auto ind2 = glm::vec4(scene.GetVertex(z), 1);

    ind0 = matrix * ind0;
    ind1 = matrix * ind1;
    ind2 = matrix * ind2;

//    if (texID != -1 && !scene.GetTexture(texID).IsPerlin())
//    {
//        return Triangle{Vertex{x, {ind0.x, ind0.y, ind0.z}, {0, 0, 0}, scene.Get_UV(a + texCoordsOffset)},
//                        Vertex{y, {ind1.x, ind1.y, ind1.z}, {0, 0, 0}, scene.Get_UV(b + texCoordsOffset)},
//                        Vertex{z, {ind2.x, ind2.y, ind2.z}, {0, 0, 0}, scene.Get_UV(c + texCoordsOffset)},
//                        scene.GetMaterial(matID), texID, index, smooth};
    return gpt::shapes::Triangle(index,
                                 {ind0.x, ind0.y, ind0.z},
                                 {ind1.x, ind1.y, ind1.z},
                                 {ind2.x, ind2.y, ind2.z},
                                 scene.GetMaterial(matID));
//    }
//    else
//    {
//        return Triangle{Vertex{x, {ind0.x, ind0.y, ind0.z}},
//                        Vertex{y, {ind1.x, ind1.y, ind1.z}},
//                        Vertex{z, {ind2.x, ind2.y, ind2.z}},
//                        scene.GetMaterial(matID), texID, index, smooth};
//    }
}

std::vector<gpt::shapes::Mesh> gpt::shapes::Mesh::Load(gpt::Scene& scene, tinyxml2::XMLElement *elem)
{
    std::vector<gpt::shapes::Mesh> meshes;

    for (auto child = elem->FirstChildElement("Mesh"); child != nullptr; child = child->NextSiblingElement("Mesh")) {
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
        if(auto trns = child->FirstChildElement("Transformations")){
            std::istringstream ss {trns->GetText()};
            transformations = GetTransformations(ss);
        }

        glm::mat4 matrix(1.0f);
        for (auto& tr : transformations){
            auto m = scene.GetTransformation(tr);
            matrix = m * matrix;
        }

        Mesh msh {id, scene.GetMaterial(matID)};
        auto FaceData = child->FirstChildElement("Faces");
        std::istringstream stream { FaceData->GetText() };
        int vertexOffset = 0;
        int texCoordOffset = 0;

        if (FaceData->QueryIntAttribute("vertexOffset", &vertexOffset))
            ;
        if (FaceData->QueryIntAttribute("textureOffset", &texCoordOffset))
            ;

        boost::optional<Triangle> tr;
        int index = 1;

//        gpt::shapes::ShadingMode mode = gpt::shapes::ShadingMode::Flat;
//        const char* asd = child->Attribute("shadingMode");
//        if (asd)
//        {
//            std::string sm = std::string(asd);
//            if (sm == "smooth")
//            {
//                mode = gpt::shapes::ShadingMode::Smooth;
//            }
//        }

        while((tr = GetFace(scene, stream, vertexOffset, texCoordOffset, matrix, matID, index++, texID))){
            auto tri = *tr;
            msh.AddFace(std::move(*tr));
        }

//        msh.SetShadingMode(mode);
//        msh.SetArtificial(is_art);

//        msh.BoundingBox();
        meshes.push_back(std::move(msh));

    }


//    for (auto& mesh : meshes)
//    {
//        assert(mesh.GetMaterial());
//    }

    return meshes;
}