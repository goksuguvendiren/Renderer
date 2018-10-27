#include <iostream>
#include <string>
#include <shapes/Sphere.hpp>
#include <shapes/Triangle.hpp>
#include <shapes/Mesh.hpp>
#include <Scene.hpp>
#include <Camera.hpp>

#include <opencv2/opencv.hpp>

#define CATCH_CONFIG_MAIN
#define main ___MAIN
#include "catch2/catch.hpp"
#undef main

//int main()
//{
//    Catch::Session().run();
//}
//
//int x_main(int argc, char** argv)
int main()
{
    gpt::Scene scene;
    scene.Load("/Users/goksu/Documents/Renderer/inputs/cornellbox_ldr.xml");

//    scene.AddShape(std::make_unique<gpt::shapes::Sphere>(0, 0.3, glm::vec3{-0.875, 1, -2}));
//    scene.AddShape(std::make_unique<gpt::shapes::Sphere>(0, 0.3, glm::vec3{-0.875, 1, -2}));
//    scene.AddShape(std::make_unique<gpt::shapes::Sphere>(0, 0.3, glm::vec3{-0.875, 1, -2}));

    auto image = gpt::Render(scene);
    cv::Mat im = cv::Mat(image.Height(), image.Width(), CV_32FC3);

    for (int i = 0; i < image.Height(); i++){
        for (int j = 0; j < image.Width(); j++){
            // ATTENTION : OpenCV expects BGR color space
            im.at<cv::Vec3f>(i, j)[0] = image.at(i, j).b;// / 255;
            im.at<cv::Vec3f>(i, j)[1] = image.at(i, j).g;// / 255;
            im.at<cv::Vec3f>(i, j)[2] = image.at(i, j).r;// / 255;
        }
    }

    cv::imshow("result", im);
    cv::waitKey();

    return 0;
}