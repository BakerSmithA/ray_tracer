#include <glm/glm.hpp>

#ifndef NOISE_2D_H
#define NOISE_2D_H

// Generates a random 2D noise texture.
class Noise2d: public BilinearTexture2d {
private:
    const int m_width, m_height;
    float *buffer;

public:
    // param width: the width of the generated texture.
    // param height: the height of the generated texture.
    // param seed: the seed used when generating random numbers.
    Noise2d(int width, int height, int seed = 0): m_width(width), m_height(height) {
        this->buffer = new float[m_width * m_height];
        Noise2d::fill_random(m_width, m_height, this->buffer);
    }

    // return: the color of the pixel at the given x, y in the space of the
    //         image buffer. Therefore the x and y go from 0 to image width
    //         and height respectively.
    vec3 pixel_at(int x, int y) const {
        if (x >= this->m_width) {
            int min_x = min((int)floor(x), this->width()-1);
            int min_y = min((int)floor(y), this->height()-1);

            int max_x = min((int)ceil(x), this->width()-1);
            int max_y = min((int)ceil(y), this->height()-1);

            printf("min_x = %d\nmax_x = %d\nmin_y = %d\nmax_y = %d\n\n", min_x, max_x, min_y, max_y);
        }

        float grayscale = this->buffer[y * this->m_width + x];
        return vec3(grayscale);
    }

    // return: the width of the image buffer.
    int width() const {
        return this->m_width;
    }

    // return: the height of the image buffer.
    int height() const {
        return this->m_height;
    }

private:
    // effect: populates noise with width * height random numbers from 0-1.
    static void fill_random(int width, int height, float *buffer) {
        for (int i=0; i<width * height; i++) {
            buffer[i] = glm::linearRand(0.0f, 1.0f);
        }
    }
};

#endif // NOISE_2D_H
