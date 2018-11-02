//
// Created by Göksu Güvendiren on 21/07/2018.
//

#include "Camera.hpp"
#include "Scene.hpp"
#include <iostream>
#include <sstream>

#include "Utils.hpp"
#include <random>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/constants.hpp>

std::mt19937 hemisphere_seed;

glm::vec3 sample_hemisphere(const glm::vec3& normal)
{
    static std::uniform_real_distribution<float> asd(0, 1);
    static std::uniform_real_distribution<float> asd1(0, 1);
    float sample1 = asd(hemisphere_seed);
    float sample2 = asd1(hemisphere_seed);

    auto dir = glm::vec3{cos(2 * glm::pi<float>() * sample2) * glm::sqrt(1 - glm::pow(sample1, 2.f)),
                         sample1,
                         sin(2 * glm::pi<float>() * sample2) * glm::sqrt(1 - glm::pow(sample2, 2.f))};

    auto c = glm::cross(glm::vec3{0.f, 1.f, 0.f}, normal);
    auto angle = glm::acos(glm::dot(glm::vec3{0, 1, 0}, normal));
    auto res = glm::angleAxis(angle, c) * glm::normalize(dir);

    const float r = std::sqrt(sample1);
    const float theta = 2 * glm::pi<float>() * sample2;

    const float x = r * std::cos(theta);
    const float y = r * std::sin(theta);

    return glm::vec3(x, y, std::sqrt(std::max(0.0f, 1 - sample1)));

    if (normal == glm::vec3{0, -1, 0})
    {
        return -res;
    }
    return res;
}


glm::vec3 Trace(const gpt::Scene& scene, const gpt::Ray& ray, int recursionDepth, int maxRecDepth)
{
    glm::vec3 color = {0, 0, 0};

    boost::optional<gpt::HitInfo> hit = scene.Hit(ray);

    if (hit)
    {
        color = hit->Material().CalculateReflectance(scene, *hit, 1);
    }
    else
    {
        color = scene.BackgroundColor();
        return color;
    }

    if (recursionDepth >= maxRecDepth) return color;

    // otherwise, redirect the ray :
    auto direction = sample_hemisphere(hit->Normal());
    gpt::Ray monte_carlo_ray(hit->Position() + (scene.ShadowRayEpsilon() * direction), direction, false);
    return color + Trace(scene, monte_carlo_ray, recursionDepth++, maxRecDepth);
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

            auto ray = gpt::Ray(camera.Position(), pixelLocation - camera.Position(), true);
            auto color = Trace(scene, ray, 1, 1);

            image.at(i, j) = glm::min(color, glm::vec3{255.f, 255.f, 255.f}) / 255.f;
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