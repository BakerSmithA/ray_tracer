#include "light.h"

#ifndef SHADOW_LIGHT_H
#define SHADOW_LIGHT_H

// Models a light which can cast shadows.
class ShadowLight: public Light {
public:
    ShadowLight(vec3 color): Light(color) {
    }

    // param point: the point to be illuminated.
    // param surface_normal: the normal of the object at the point.
    // return: the intensity of each color channel of the light at the given
    //         position in the scene.
    virtual vec3 intensity(vec4 point, vec4 surface_normal) const = 0;

    // return: a shadow ray from the point and the light source.
    virtual Ray shadow_ray_from(vec4 point) const = 0;

    // return: a number of randomly selected shadow rays from an area around
    //         the light to the point.
    virtual vector<Ray> random_shadow_rays_from(vec4 point, int num) const = 0;
};

#endif // SHADOW_LIGHT_H
