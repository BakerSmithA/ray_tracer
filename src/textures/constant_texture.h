#include "texture.h"

#ifndef CONSTANT_TEXTURE
#define CONSTANT_TEXTURE

template <class ObjectRelativePos>
class ConstantTexture: public Texture<ObjectRelativePos> {
public:
    // The paramters for everywhere in the texture.
    const vec3 color;
    const float transparency;

    ConstantTexture(vec3 color, float transparency):
        color(color), transparency(transparency) {
    }

    // return: the color of the the texture at the given position in the
    //         coordinate space of the object, i.e. in the range 0-1 for each
    //         axis.
    vec3 color_at(ObjectRelativePos position) const override {
        return this->color;
    }

    // return: the transparency of the the texture at the given position in the
    //         coordinate space of the object, i.e. in the range 0-1 for each
    //         axis.
    float transparency_at(ObjectRelativePos position) const override {
        return this->transparency;
    }
};

#endif // CONSTANT_TEXTURE
