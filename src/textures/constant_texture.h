#include "texture.h"

#ifndef CONSTANT_TEXTURE
#define CONSTANT_TEXTURE

template <class ObjectRelativePos>
class ConstantTexture: public Texture<ObjectRelativePos> {
public:
    // The paramters for everywhere in the texture.
    const vec3 color;
    const float density;

    ConstantTexture(vec3 color, float density):
        color(color), density(density) {
    }

    // param position: position in the texture where each axis goes from 0-1.
    // return: the color of the the texture at the given position in the
    //         coordinate space of the object.
    vec3 color_at(ObjectRelativePos position) const override {
        return this->color;
    }

    // param position: position in the texture where each axis goes from 0-1.
    // return: the density of the the texture at the given position in the
    //         coordinate space of the object. The denser this is, the less
    //         light will be allowed to pass through.
    float density_at(ObjectRelativePos position) const override {
        return this->density;
    }
};

#endif // CONSTANT_TEXTURE
