//
// Created by Göksu Güvendiren on 21/07/2018.
//

#include "Camera.hpp"
#include "Scene.hpp"
#include <iostream>
#include <sstream>

#include "Utils.hpp"

glm::vec3 Trace(const gpt::Scene& scene, glm::vec3 origin, glm::vec3 direction, bool primary, int recursionDepth, int maxRecDepth)
{
    glm::vec3 color = {0, 0, 0};

    auto ray = gpt::Ray(origin, direction, primary);
    boost::optional<gpt::HitInfo> hit = scene.Hit(ray);

    if (hit)
    {
        color = hit->Material().CalculateReflectance(scene, *hit, 1);
    }
    else
    {
        color = scene.BackgroundColor();
    }


    if (recursionDepth >= maxRecDepth) return color;

    // otherwise, redirect the ray :
//    auto direction = sample_hemisphere(hit.Normal());
//    return color + Trace(hit->Position() + (scene.ShadowRayEpsilon() * direction), direction, false, recursionDepth--, maxRecDepth)
    return color;
}


glm::vec3 RenderPixel(const gpt::Scene& scene, const gpt::Camera& camera, const glm::vec3 &pixelLocation)
{
    glm::vec3 pixelColor = {0, 0, 0};
    glm::vec3 cameraLocation = camera.Position();

//    if (hit)
    {
        glm::vec3 col = Trace(scene, cameraLocation, pixelLocation - cameraLocation, true, 1, 1);
                //hit->Material().CalculateReflectance(scene, *hit, 1); //{255, 230, 234};//CalculateMaterialReflectances(*hit, 0);
        pixelColor += glm::min(col, glm::vec3{255.f, 255.f, 255.f}) / 255.f;
    }
//    else
//    {
//        pixelColor += scene.BackgroundColor();
//    }

    return pixelColor;
}

glm::vec3 CalculatePixelLocation(const gpt::Camera& camera, glm::vec3 pixelCenter)
{
    return pixelCenter;
}

void UpdateProgress(float progress)
{
    int barWidth = 70;

    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
};

gpt::Image gpt::Render(/*const gpt::Camera& camera, */const gpt::Scene& scene)
{
    int index = 0;
    auto& camera = scene.GetCamera(index);
    gpt::Image image(camera.ImagePlane().NX(), camera.ImagePlane().NY());

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

        UpdateProgress(i / (float)camera.ImagePlane().NY());
    }

    return image;
}

gpt::Camera gpt::LoadCamera(tinyxml2::XMLElement *element)
{
    int id;
    if (element->QueryIntAttribute("id", &id) == tinyxml2::XML_NO_ATTRIBUTE){
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