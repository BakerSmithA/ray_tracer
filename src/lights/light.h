#include "../geometry/ray.h"

#ifndef LIGHT_H
#define LIGHT_H

class Light {
public:
    const vec3 color;

    Light(vec3 color): color(color) {
    }

    // param point: the point to be illuminated.
    // param surface_normal: the normal of the object at the point.
    // return: the intensity of light at the position in the scene.
    virtual vec3 intensity(vec4 point, vec4 surface_normal) const = 0;
};

#endif // LIGHT_H
