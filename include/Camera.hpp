//
// Created by Göksu Güvendiren on 21/07/2018.
//

#pragma once

#include <glm/glm.hpp>
#include <string>
#include "ImagePlane.hpp"
#include "Image.hpp"

namespace gpt
{
    class Scene;
    class Camera;

    Image Render(const gpt::Scene& scene);
    glm::vec3 CalculatePixelLocation(const gpt::Camera& camera, glm::vec3 pixelCenter);

    class Camera
    {
        gpt::ImagePlane imagePlane;

//        int id;
        std::string imageName;

        glm::vec3 position;
        glm::vec3 planePosition;
        int sampleCount;
        int divCount;

//        int focalDistance;
//        float apertureSize;

        glm::vec3 up;
        glm::vec3 gaze;
        glm::vec3 right;


    public:
        Camera(const gpt::ImagePlane& plane = {} /*, int i = 0*/, glm::vec3 p = {0, 0, 0},
               glm::vec3 g = {0, 1, 0},
               glm::vec3 u = {0, 0, 1},
               std::string name = "",
               int numSamp = 1
             /*  int fd = 1, float as = 0*/) : imagePlane(plane),
                                           /*id(i),*/ imageName(std::move(name)),
                                           position(p), sampleCount(numSamp),
                                           divCount((int)std::floor(std::sqrt(numSamp)))/*,
                                           focalDistance(fd), apertureSize(as)*/
        {
            up    = glm::normalize(u);
            gaze  = glm::normalize(g);
            right = glm::normalize(glm::cross(gaze, up));
            up    = glm::normalize(glm::cross(right, gaze));

            planePosition = position + imagePlane.Left() * right + imagePlane.Top() * up + imagePlane.DistanceToCamera() * gaze;
        };

//        glm::vec3 GetCameraPosition() const;

        glm::vec3 Position() const { return position; }
        glm::vec3 Up() const { return up; }
        glm::vec3 Gaze() const { return gaze; }
        glm::vec3 Right() const { return right; }

        int SampleCount() const { return sampleCount; }
        int DivCount() const { return divCount; }

        gpt::ImagePlane ImagePlane() const { return imagePlane; }

        glm::vec3 PlanePosition() const { return planePosition; }

        void SetSampleCount(int numsamp) { sampleCount = numsamp; }
    };
}
