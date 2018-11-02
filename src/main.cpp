#include <iostream>
#include <string>
#include <shapes/Sphere.hpp>
#include <Scene.hpp>
#include <Camera.hpp>

#include <opencv2/opencv.hpp>
#include <loaders/xml_loader.hpp>

int main()
{
    gpt::Scene scene = load_scene("/Users/goksu/Documents/Renderer/inputs/cornellbox_ldr.xml");

    auto image = gpt::Render(scene);
    cv::Mat im = cv::Mat(image.Height(), image.Width(), CV_32FC3);

    for (int i = 0; i < image.Height(); i++)
    {
        for (int j = 0; j < image.Width(); j++)
        {
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