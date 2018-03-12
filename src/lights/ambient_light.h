#include "light.h"

#ifndef AMBIENT_LIGHT_H
#define AMBIENT_LIGHT_H

// Models a light which cannot cast shadows, and has the same intensity
// everywhere/
class AmbientLight: public Light {
public:
    AmbientLight(vec3 color): Light(color) {
    }

    // An ambient light has no position, and so its intensity is the same
    // everywhere.
    //
    // param point: the point to be illuminated.
    // param surface_normal: the normal of the object at the point.
    // return: the intensity of each color channel of the light at the given
    //         position in the scene.
    virtual vec3 intensity(vec4 point, vec4 surface_normal) const {
        return this->color;
    }
};

#endif // AMBIENT_LIGHT_H
