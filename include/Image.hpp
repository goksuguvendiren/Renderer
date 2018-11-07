//
// Created by Göksu Güvendiren on 22/07/2018.
//

#pragma once

#include <vector>
#include <glm/vec3.hpp>

namespace gpt
{
    class Image
    {
        std::vector<glm::vec3> pixels;

        unsigned int width;
        unsigned int height;

    public :
        Image() : width(0), height(0) {}
        Image(unsigned int w, unsigned int h) : width(w), height(h) {
            pixels.resize(width * height);
        }

        glm::vec3& at(int i, int j) { return pixels[i * width + j]; }
        glm::vec3  at(int i, int j) const { return pixels[i * width + j]; }

        unsigned int Width() const { return width; }
        unsigned int Height() const { return height; }

        const std::vector<glm::vec3>& Data() { return pixels; }
    };
}
