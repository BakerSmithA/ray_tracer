#include <glm/glm.hpp>
#include "buffer_2d.h"

#ifndef PERLIN_2D_H
#define PERLIN_2D_H

// Generates a random 2D noise texture.
class Perlin2d: public Texture<vec2> {
private:
    // The number of noise images to combine.
    const int octaves;
    // The width and height of the noise texture.
    const int side_length;
    // Stores the noise which is used to initialise the noise textures.
    vec3 *buffer = NULL;
    // Added together to compose the final noise texture.
    vector<Buffer2d*> noise_textures;

public:
    // param width: the width of the generated texture.
    // param height: the height of the generated texture.
    // param seed: the seed used when generating random numbers.
    Perlin2d(int octaves, bool use_red_as_alpha = false, int seed = 0):
        // Texture2d(use_red_as_alpha), octaves(octaves), side_length(1 << octaves)
        octaves(octaves), side_length(1 << octaves)
    {
        this->buffer = new vec3[side_length * side_length];
        Perlin2d::fill_random(side_length, this->buffer);

        // Create the noise textures.
        for (int i=0; i<this->octaves; i++) {
            int octave_side_length = 1 << i; // 2^i
            noise_textures.push_back(new Buffer2d(octave_side_length, octave_side_length, this->buffer));
        }
    }

    ~Perlin2d() {
        // for (int i=0; i<octaves; i++) {
        //     delete this->noise_textures[i];
        // }
        //
        // delete[] this->buffer;
        // this->buffer = NULL;
    }

    // param position: position in the texture where each axis goes from 0-1.
    // return: the color of the the texture at the given position in the
    //         coordinate space of the object.
    vec3 color_at(vec2 position) const {
        // Averages the values of noise for all noise textures.
        vec3 col = vec3(0.0f, 0.0f, 0.0f);
        for (int i=0; i<this->octaves; i++) {
            col += noise_textures[i]->color_at(position);
        }
        return col / (float)octaves;
    }

private:
    // effect: populates noise with width * height random numbers from 0-1.
    static void fill_random(int side_length, vec3 *buffer) {
        for (int i=0; i<side_length * side_length; i++) {
            float r = glm::linearRand(0.0f, 1.0f);
            buffer[i] = vec3(r, r, r);
        }
    }
};

#endif // NOISE_2D_H
