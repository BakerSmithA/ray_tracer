#include <glm/glm.hpp>

#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

// Interface for @D textures filling a space from 0-1 in each axis.
class Texture2d {
public:
    // return: the width of the texture.
    virtual int width() = 0;

    // return: the height of the texture.
    virtual int height() = 0;

    // param position: position in the texture where each axis goes from 0-1.
    // return: the color of the the texture at the given position in the
    //         coordinate space of the object.
    virtual vec3 color_at(vec2 position) const = 0;
};

#endif // TEXTURE_2D_H
