//
// Created by Göksu Güvendiren on 21/07/2018.
//

#include "Camera.hpp"
#include "Scene.hpp"
#include <iostream>
#include <sstream>
#include <opencv/cv.hpp>

#include "Utils.hpp"

glm::vec3 Trace(const gpt::Scene& scene, const gpt::Ray& ray, int recursionDepth, int maxRecDepth)
{
    glm::vec3 color = glm::vec3{0.f, 0.f, 0.f};

    boost::optional<gpt::HitInfo> hit = scene.Hit(ray);

    if (hit)
    {
        if (hit->Material().Terminate()) return hit->Material().CalculateReflectance(-ray.Direction(), {}, hit->Normal());

        for (auto& light : scene.Lights())
        {
            auto lightDirection = light->Direction(hit->Position());
            auto normalLightDirection = glm::normalize(lightDirection);
            gpt::Ray shadowRay(hit->Position() + (scene.ShadowRayEpsilon() * hit->Normal()), normalLightDirection);

            boost::optional<gpt::HitInfo> sh;
            if ((sh = scene.Hit(shadowRay)))
            {
                if (sh->Param() < glm::length(lightDirection))
                    continue;
            }

            color += hit->Material().CalculateReflectance(-ray.Direction(), lightDirection, hit->Normal()) * light->Intensity(lightDirection);
        }

        if (recursionDepth >= maxRecDepth) return color;

        auto direction = gpt::utils::sample_hemisphere(hit->Normal());
        gpt::Ray monte_carlo_ray(hit->Position() + (scene.ShadowRayEpsilon() * direction), direction, false);
        auto sampledColor = Trace(scene, monte_carlo_ray, recursionDepth + 1, maxRecDepth);

        return color + hit->Material().CalculateReflectance(-ray.Direction(), direction, hit->Normal()) * sampledColor;
    }
    else
    {
        if (!ray.IsPrimary())
        {
            volatile int x = 10;
        }
        color = scene.BackgroundColor();
        return color;
    }
}

glm::vec3 gpt::CalculatePixelLocation(const gpt::Camera& camera, glm::vec3 pixelCenter)
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

gpt::Image SubRender(const gpt::Scene& scene, const gpt::Camera& camera)
{
    gpt::Image image(camera.ImagePlane().NX(), camera.ImagePlane().NY());

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

            glm::vec3 color = Trace(scene, ray, 0, 4);
            image.at(i, j) = color / 255.f;//glm::min(color, glm::vec3{255.f, 255.f, 255.f}) / 255.f;
        }

        UpdateProgress(i / (float)camera.ImagePlane().NY());
    }
    std::cerr << '\n';
    return image;
}

gpt::Image gpt::Render(const gpt::Scene& scene)
{
    int index = 0;
    auto& camera = scene.GetCamera(index);
    gpt::Image accImage(camera.ImagePlane().NX(), camera.ImagePlane().NY());
    gpt::Image resultImage(camera.ImagePlane().NX(), camera.ImagePlane().NY());

    auto key = 0;
    auto frames = 0;
    while (key != 27)
    {
        std::cerr << "Rendering the frame: " << frames << '\n';

        auto newimage = SubRender(scene, camera);
        accImage += newimage;
        resultImage = accImage;

        cv::Mat im = cv::Mat(resultImage.Height(), resultImage.Width(), CV_32FC3, (void*)&(resultImage.Data()[0].x));
        cv::cvtColor(im, im, CV_RGB2BGR);

        im /= ++frames;
        cv::imshow("frame", im);
        key = cv::waitKey(10);

        std::cerr << "key is : " << key << '\n';
    }

    return resultImage;
}