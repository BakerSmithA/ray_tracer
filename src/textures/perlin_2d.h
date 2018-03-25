#include <glm/glm.hpp>

#ifndef PERLIN_2D_H
#define PERLIN_2D_H

// Generates a 2d perlin noise texture.
class Perlin2d: public Texture2d {
private:
    const vec3 color;
    const int width, height;
    float *noise_buffer;

public:
    // param width: the width of the generated texture.
    // param height: the height of the generated texture.
    // param seed: the seed used when generating random numbers.
    Perlin2d(vec3 color, int width, int height, int seed = 0): color(color), width(width), height(height) {
        this->noise_buffer = new float[width * height];
        Perlin2d::fill_random(width, height, this->noise_buffer);
    }

    // param position: position in the texture where each axis goes from 0-1.
    // return: the color of the the texture at the given position in the
    //         coordinate space of the object.
    virtual vec3 color_at(vec2 position) const {
        vec2 uv = position * vec2(this->width, this->height);
        float noise_val = this->noise_buffer[(int)uv.x * this->width + (int)uv.y];
        return this->color * noise_val;
    }

private:
    // effect: populates noise with width * height random numbers from 0-1.
    static void fill_random(int width, int height, float *noise_buffer) {
        for (int i=0; i<width * height; i++) {
            noise_buffer[i] = glm::linearRand(0.0f, 1.0f);
        }
    }
};

#endif // PERLIN_2D_H
