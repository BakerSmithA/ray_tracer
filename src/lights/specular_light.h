#include "light.h"

#ifndef SPECULAR_LIGHT_H
#define SPECULAR_LIGHT_H

// Models a light which can cast shadows.
class SpecularLight: public Light {
public:
    SpecularLight(vec3 color): Light(color) {
    }

    // param point: the point to be illuminated.
    // param surface_normal: the normal of the object at the point.
    // return: the intensity of each color channel of the light at the given
    //         position in the scene.
    virtual vec3 intensity(vec4 point, vec4 surface_normal) const = 0;

    // return: a ray from the point and the light source.
    virtual Ray ray_from(vec4 point) const = 0;

    // return: a number of randomly selected shadow rays from an area around
    //         the light to the point.
    virtual vector<Ray> random_shadow_rays_from(vec4 point, int num) const = 0;
};

#endif // SPECULAR_LIGHT_H
