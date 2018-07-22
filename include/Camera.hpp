//
// Created by Göksu Güvendiren on 21/07/2018.
//

#ifndef RAYTRACER_CAMERA_HPP
#define RAYTRACER_CAMERA_HPP

#include <glm/glm.hpp>
#include <string>
#include "ImagePlane.hpp"
#include "Image.hpp"
//#include "Camera.hpp"
//#include "Scene.hpp"

namespace gpt
{
    class Scene;
    class Camera;

    Image Render(/*const gpt::Camera& camera,*/ const gpt::Scene& scene);
    gpt::Camera LoadCamera(tinyxml2::XMLElement* element);

    class Camera
    {
        gpt::ImagePlane imagePlane;

        int id;
        std::string imageName;

        glm::vec3 position;
        glm::vec3 planePosition;
        int sampleCount;
        int divCount;

        int focalDistance;
        float apertureSize;

        glm::vec3 up;
        glm::vec3 gaze;
        glm::vec3 right;


    public:
        Camera(const gpt::ImagePlane& plane = {} , int i = 0, glm::vec3 p = {0, 0, 0},
               glm::vec3 g = {0, 1, 0},
               glm::vec3 u = {0, 0, 1},
               std::string name = "",
               int numSamp = 1,
               int fd = 1, float as = 0) : imagePlane(plane),
                                           id(i), imageName(name),
                                           position(p), sampleCount(numSamp),
                                           divCount(std::floor(std::sqrt(numSamp))),
                                           focalDistance(fd), apertureSize(as)
        {
            up    = glm::normalize(u);
            gaze  = glm::normalize(g);
            right = glm::normalize(glm::cross(gaze, up));
            up    = glm::normalize(glm::cross(right, gaze));

            planePosition = position + imagePlane.Left() * right + imagePlane.Top() * up + imagePlane.DistanceToCamera() * gaze;
        };

//        glm::vec3 GetCameraPosition() const;
        glm::vec3 RenderPixel(const glm::vec3& pixelcenter) const;

        glm::vec3 Position() const { return position; }
        glm::vec3 Up() const { return up; }
        glm::vec3 Gaze() const { return gaze; }
        glm::vec3 Right() const { return right; }

        int SampleCount() const { return sampleCount; }
        int DivCount() const { return divCount; }

        gpt::ImagePlane ImagePlane() const { return imagePlane; }

        glm::vec3 PlanePosition() const { return planePosition; }
        glm::vec3 GetPixelLocation(int i, int j) const;

        void SetSampleCount(int numsamp) { sampleCount = numsamp; }
    };
}

#endif //RAYTRACER_CAMERA_HPP
