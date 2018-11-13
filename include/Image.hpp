//
// Created by Göksu Güvendiren on 22/07/2018.
//

#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <boost/iterator/zip_iterator.hpp>

namespace gpt
{
    class Image
    {
        std::vector<glm::vec3> pixels;

        unsigned int width;
        unsigned int height;

    public :
        Image() : width(0), height(0) {}
        Image(unsigned int w, unsigned int h) : width(w), height(h)
        {
            pixels.resize(width * height);
        }

        glm::vec3& at(int i, int j) { return pixels[i * width + j]; }
        glm::vec3  at(int i, int j) const { return pixels[i * width + j]; }

        unsigned int Width() const { return width; }
        unsigned int Height() const { return height; }

        void operator+=(const Image& rhs)
        {
            std::transform(boost::make_zip_iterator(boost::make_tuple(pixels.begin(), rhs.pixels.begin())),
                           boost::make_zip_iterator(boost::make_tuple(pixels.end(), rhs.pixels.end())),
                           pixels.begin(),
            [](const boost::tuple<const glm::vec3&, const glm::vec3&>& its)
            {
                return its.get<0>() + its.get<1>();
            });
        }

        const std::vector<glm::vec3>& Data() { return pixels; }
    };
}
