#include "texture.h"

#ifndef CONSTANT_TEXTURE
#define CONSTANT_TEXTURE

template <class Position>
class ConstantTexture: public Texture<Position> {
public:
    // The paramters for everywhere in the texture.
    const vec3 color;
    const float transparency;

    ConstantTexture(vec3 color, float transparency):
        color(color), transparency(transparency) {
    }

    // return: the color of the the texture at the given position in 3D space.
    vec3 color_at(Position position) const override {
        return this->color;
    }

    // return: the transparency of the the texture at the given position in 3D space.
    float transparency_at(Position position) const override {
        return this->transparency;
    }
};

#endif // CONSTANT_TEXTURE
