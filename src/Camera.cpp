//
// Created by Göksu Güvendiren on 21/07/2018.
//

#include "Camera.hpp"
#include "Scene.hpp"
#include <iostream>
#include <sstream>

glm::vec3 RenderPixel(const gpt::Scene& scene, const gpt::Camera& camera, const glm::vec3 &pixelLocation)
{
    glm::vec3 pixelColor = {0, 0, 0};
    glm::vec3 cameraLocation = camera.Position();

    auto ray = gpt::Ray(cameraLocation, glm::normalize(pixelLocation - cameraLocation), true);
    boost::optional<gpt::HitInfo> hit = scene.Hit(ray);
//        auto sth = *hit;

    if (hit)
    {
        glm::vec3 col = {255, 230, 234};//CalculateMaterialReflectances(*hit, 0);
        pixelColor += glm::min(col, glm::vec3{500.f, 500.f, 500.f});
    }
    else
    {
        pixelColor += glm::vec3{0, 0, 0};//scene.BackgroundColor();
    }

    return pixelColor;
}

glm::vec3 CalculatePixelLocation(const gpt::Camera& camera, glm::vec3 pixelCenter)
{
    return pixelCenter;
//    float cellWidth  = camera.ImagePlane().PixelWidth()  / float(camera.DivCount());
//    float cellHeight = camera.ImagePlane().PixelHeight() / float(camera.DivCount());
//
//    auto oneRight = cellWidth * camera.Right();
//    auto oneDown  = -cellHeight * camera.Up();
//
//    // label the first pixel, by taking the center of the pixel to the top left
//    glm::vec3 pixelBeginning = pixelCenter - ((camera.ImagePlane().PixelWidth() / 2.f) * camera.Right());
//    pixelBeginning -= ((camera.ImagePlane().PixelHeight() / 2.f) * camera.Up());
//
//    // then add half a cell size to locate the center of the cell.
//    pixelBeginning += cellWidth / 2.f;
//    pixelBeginning += cellHeight / 2.f;
//
//    //calculate pixel location
//    int indH = camera.DivCount();
//    int indW = camera.DivCount();
//
//    glm::vec3 pixelLocation = pixelBeginning + (float(indW) * oneRight + float(indH) * oneDown);
//
//    std::cerr << (pixelLocation == pixelCenter) << '\n';
//
//    return pixelLocation;
}

gpt::Image gpt::Render(/*const gpt::Camera& camera, */const gpt::Scene& scene)
{
    int index = 0;
    auto& camera = scene.GetCamera(index);
    gpt::Image image(camera.ImagePlane().NX(), camera.ImagePlane().NY());

//    auto grsamples = samples;

    std::cerr << "Rendering with " << camera.SampleCount() << " samples for pixels." << '\n';

    auto oneRight = camera.ImagePlane().PixelWidth() * camera.Right();
    auto oneDown  = -camera.ImagePlane().PixelHeight() * camera.Up();

    auto pixLocation = camera.PlanePosition();  // the position of the top left corner of the image plane.
    pixLocation -= oneRight * 0.5f;             // move the vector half a pixel left and up so that it will be easier in the loop.
    pixLocation -= oneDown * 0.5f;

    auto rowBeginning = pixLocation;
    for (int i = 0; i < camera.ImagePlane().NY(); i++)
    {
        rowBeginning += oneDown;
        auto pixelCenter = rowBeginning;
        for (int j = 0; j < camera.ImagePlane().NX(); j++)
        {
            pixelCenter += oneRight;
            auto pixelLocation = CalculatePixelLocation(camera, pixelCenter);

            image.at(i, j) = RenderPixel(scene, camera, pixelLocation);
        }
//
//        auto progress = i / (float)camera.ImagePlane().NY();
//
//        auto UpdateProgress = [](float progress)
//        {
//            int barWidth = 70;
//
//            std::cout << "[";
//            int pos = barWidth * progress;
//            for (int i = 0; i < barWidth; ++i) {
//                if (i < pos) std::cout << "=";
//                else if (i == pos) std::cout << ">";
//                else std::cout << " ";
//            }
//            std::cout << "] " << int(progress * 100.0) << " %\r";
//            std::cout.flush();
//        };
//
//        UpdateProgress(progress);
    }
    std::cout << std::endl;

    return image;
}

gpt::Camera gpt::LoadCamera(tinyxml2::XMLElement *element)
{
    int id;
    if (element->QueryIntAttribute("id", &id) == tinyxml2::XML_NO_ATTRIBUTE){
        std::cerr << "No such attribute as id" << '\n';
        std::abort();
    }

    auto GetElem = [](tinyxml2::XMLElement* element)
    {
        glm::vec3 color;

        std::istringstream ss {element->GetText()};
        ss >> color.r;
        ss >> color.g;
        ss >> color.b;

        return color;
    };

    glm::vec3 position = GetElem(element->FirstChildElement("Position"));
    glm::vec3 gaze = GetElem(element->FirstChildElement("Gaze"));
    glm::vec3 up = GetElem(element->FirstChildElement("Up"));

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