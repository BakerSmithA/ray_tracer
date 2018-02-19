#include <math.h>
#include "light.h"

#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

// Models a point light which radiates light outwards.
class PointLight: public Light {
public:
    const vec4 pos;

    PointLight(vec3 color, vec4 pos): Light(color), pos(pos) {
    }

    // param point: the point to be illuminated.
    // param surface_normal: the normal of the object at the point.
    // return: the intensity of light at the position in the scene.
    vec3 intensity(vec4 point, vec4 surface_normal) const override {
        // The distance from the light source to the intersection, i.e. the radius of the sphere.
        vec4 shadow_ray = this->pos - point;
        // The intensity of light is inversely proportional to the distance squared.
        float dist = length(shadow_ray);
        float surface_area_at_radius_r = 4 * M_PI * dist * dist;
        vec3 intensity = this->color / surface_area_at_radius_r;

        return intensity;
    }

    // return: whether the light casts shadows.
    bool does_cast_shadows() const override {
        return true;
    }

    // return: a shadow ray from the point and the light source. This is
    //         only used if the light casts shadows.
    Ray shadow_ray_to(vec4 point) const override {
        // The ray can only be used to check obstructions between the point and
        // light. Therefore it cannot bounce.
        return Ray(point, this->pos - point, 0);
    }
};

#endif // POINT_LIGHT_H
