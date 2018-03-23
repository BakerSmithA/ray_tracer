#include <glm/glm.hpp>

#ifndef TEXTURE_H
#define TEXTURE_H

// Interface for textures (either 2d or 3d) filling a space from 0-1 in each
// axis.
// Note, by having transparent sections of the texture, complex shapes can also
// be described.
template <class ObjectRelativePos>
class Texture {
public:
    // return: the color of the the texture at the given position in the
    //         coordinate space of the object, i.e. in the range 0-1 for each
    //         axis.
    virtual vec3 color_at(ObjectRelativePos position) const = 0;

    // return: the transparency of the the texture at the given position in the
    //         coordinate space of the object, i.e. in the range 0-1 for each
    //         axis.
    virtual float transparency_at(ObjectRelativePos position) const = 0;
};

#endif // TEXTURE_H
