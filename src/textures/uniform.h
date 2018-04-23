#include "texture.h"

#ifndef CONSTANT_TEXTURE_H
#define CONSTANT_TEXTURE_H

template<typename Vec>
class Uniform: public Texture<Vec> {
public:
    // The paramters for everywhere in the texture.
    const float value;

    Uniform(float value): value(value) {
    }

    // param position: position in the texture where each axis goes from 0-1.
    // return: the same color everywhere in the texture.
    vec3 color_at(vec3 position) const override {
        return vec3(this->value);
    }
};

#endif // CONSTANT_TEXTURE_H
