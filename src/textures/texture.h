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
    // param position: position in the texture where each axis goes from 0-1.
    // return: the color of the the texture at the given position in the
    //         coordinate space of the object.
    virtual vec3 color_at(ObjectRelativePos position) const = 0;

    // param position: position in the texture where each axis goes from 0-1.
    // return: the density of the the texture at the given position in the
    //         coordinate space of the object. The denser this is, the less
    //         light will be allowed to pass through.
    virtual float density_at(ObjectRelativePos position) const = 0;
};

#endif // TEXTURE_H
