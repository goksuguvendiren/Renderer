//
// Created by Göksu Güvendiren on 22/07/2018.
//


#include <array>
#include <sstream>
#include <iostream>
#include <tinyxml/tinyxml2.h>
#include "ImagePlane.hpp"

//gpt::ImagePlane gpt::ReadImagePlane()
//{
//    return gpt::ImagePlane(1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 800, 800);
//}

gpt::ImagePlane gpt::CreatePlane(tinyxml2::XMLElement* element, int focalDistance)
{
    float distance = element->FirstChildElement("NearDistance")->FloatText();
    auto planeInfo = element->FirstChildElement("NearPlane")->GetText();

    auto lrbt = ([&planeInfo]
    {
        std::array<float, 4> arr;
        std::istringstream ss {planeInfo};

        ss >> arr[0];
        ss >> arr[1];
        ss >> arr[2];
        ss >> arr[3];

        return arr;
    })();

    auto resolution = element->FirstChildElement("ImageResolution")->GetText();

    auto n = ([&resolution]
    {
        std::array<unsigned int, 2> arr;
        std::istringstream ss {resolution};

        ss >> arr[0];
        ss >> arr[1];

        return arr;
    })();

    return gpt::ImagePlane(distance * focalDistance,
                      lrbt[0] * focalDistance,
                      lrbt[1] * focalDistance,
                      lrbt[2] * focalDistance,
                      lrbt[3] * focalDistance,
                      n[0], n[1]);
}