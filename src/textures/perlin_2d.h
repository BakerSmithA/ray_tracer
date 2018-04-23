#include <glm/glm.hpp>
#include "buffer.h"

#ifndef PERLIN_2D_H
#define PERLIN_2D_H

// Generates a random 2D noise texture.
template<typename Vec>
class Perlin: public Texture<Vec> {
private:
    // The number of noise images to combine.
    const int octaves;
    // The width and height of the noise texture.
    const int side_length;
    // Stores the noise which is used to initialise the noise textures.
    vec3 *buffer = NULL;
    // Added together to compose the final noise texture.
    vector<Buffer<Vec>*> noise_textures;

public:
    // param width: the width of the generated texture.
    // param height: the height of the generated texture.
    // param seed: the seed used when generating random numbers.
    Perlin(int octaves):
        octaves(octaves), side_length(1 << octaves)
    {
        int buff_length = pow(side_length, this->dims());
        this->buffer = new vec3[buff_length];
        Perlin::fill_random(side_length, buff_length, this->buffer);

        // Create the noise textures.
        for (int i=0; i<this->octaves; i++) {
            int octave_side_length = 1 << i; // 2^i
            Vec buffer_size = Vec(octave_side_length);
            noise_textures.push_back(new Buffer<Vec>(buffer_size, this->buffer));
        }
    }

    // ~Perlin2d() {
    //     for (int i=0; i<octaves; i++) {
    //         delete this->noise_textures[i];
    //     }
    //
    //     delete[] this->buffer;
    //     this->buffer = NULL;
    // }

    // param position: position in the texture where each axis goes from 0-1.
    // return: the color of the the texture at the given position in the
    //         coordinate space of the object.
    vec3 color_at(Vec position) const {
        // Averages the values of noise for all noise textures.
        vec3 col = vec3(0.0f, 0.0f, 0.0f);
        for (int i=0; i<this->octaves; i++) {
            col += noise_textures[i]->color_at(position);
        }
        return col / (float)octaves;
    }

private:
    // effect: populates noise with width * height random numbers from 0-1.
    static void fill_random(int side_length, int buff_length, vec3 *buffer) {
        for (int i=0; i<buff_length; i++) {
            float r = glm::linearRand(0.0f, 1.0f);
            buffer[i] = vec3(r);
            //buffer[i] = vec3((float)i / buff_length);
        }
    }
};

#endif // NOISE_2D_H
