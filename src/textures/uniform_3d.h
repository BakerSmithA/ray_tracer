#include "texture_3d.h"

#ifndef CONSTANT_TEXTURE_H
#define CONSTANT_TEXTURE_H

class Uniform3d: public Texture3d {
public:
    // The paramters for everywhere in the texture.
    const float density;

    Uniform3d(float density): density(density) {
    }

    // param position: position in the texture where each axis goes from 0-1.
    // return: the density of the the texture at the given position in the
    //         coordinate space of the object. The denser this is, the less
    //         light will be allowed to pass through.
    float density_at(vec4 position) const override {
        return this->density;
    }
};

#endif // CONSTANT_TEXTURE_H
