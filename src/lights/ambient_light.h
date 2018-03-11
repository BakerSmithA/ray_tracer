#include "light.h"

#ifndef AMBIENT_LIGHT_H
#define AMBIENT_LIGHT_H

class AmbientLight: public Light {
public:
    vec3 color;

    AmbientLight(vec3 color): Light(color) {
    }

    // param point: the point to be illuminated.
    // param surface_normal: the normal of the object at the point.
    // return: the intensity of each color channel of the light at the given
    //         position in the scene.
    virtual vec3 intensity(vec4 point, vec4 surface_normal) const {
        return this->color;
    }

    // return: a shadow ray from the point and the light source. Or, returns
    //         nothing if the light does not cast shadows.
    virtual optional<Ray> shadow_ray_from(vec4 point) const {
        return nullopt;
    }

    // return: a number of randomly selected shadow rays from an area around
    //         the light to the point. Or, returns nothing if the light does
    //         not cast shadows.
    virtual optional<vector<Ray>> random_shadow_rays_from(vec4 point, int num) const {
        return nullopt;
    }
};

#endif // AMBIENT_LIGHT_H
