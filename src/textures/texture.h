#include <glm/glm.hpp>

// #ifndef TEXTURE_H
// #define TEXTURE_H

// Interface for textures (either 2d or 3d). By having transparent sections of
// the texture, complex shapes can also be described.
template <class Position>
class Texture {
public:
    // return: the color of the the texture at the given position in 3D space.
    virtual vec3 color_at(Position position) const = 0;

    // return: the transparency of the the texture at the given position in 3D space.
    virtual float transparency_at(Position position) const = 0;
};

// #endif // TEXTURE_H
