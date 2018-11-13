#include <iostream>
#include <string>
#include <shapes/Sphere.hpp>
#include <Scene.hpp>
#include <Camera.hpp>

#include <opencv2/opencv.hpp>
#include <loaders/xml_loader.hpp>

static_assert(sizeof(glm::vec3) == sizeof(float) * 3, "glm::vec3's size is not 3 float!");

int main()
{
    gpt::Scene scene = load_scene("/Users/goksu/Documents/Renderer/inputs/cornellbox_ldr.xml");

    auto image = gpt::Render(scene);
    cv::Mat im = cv::Mat(image.Height(), image.Width(), CV_32FC3, (void*)&(image.Data()[0].x));
    cv::cvtColor(im, im, CV_BGR2RGB);

    cv::imshow("result", im);
    cv::waitKey();

    return 0;
}