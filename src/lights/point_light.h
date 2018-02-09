#include <math.h>

#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

class PointLight {
public:
    const vec4 pos;
    const vec3 color;

    PointLight(vec4 pos, vec3 color): pos(pos), color(color) {
    }

    // return: the intensity of the light, per unit surface area, at the given
    //         distance from the light.
    vec3 intensity(float distance) const {
        float surface_area_at_radius_r = 4 * M_PI * distance * distance;
        return this->color / surface_area_at_radius_r;
    }
};

#endif // POINT_LIGHT_H
