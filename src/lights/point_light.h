#include <math.h>
#include "light.h"

#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

class PointLight: public Light {
public:
    const vec4 pos;

    PointLight(vec3 color, vec4 pos): Light(color), pos(pos) {
    }

    // param point: the point to be illuminated.
    // param surface_normal: the normal of the object at the point.
    // return: the intensity of light at the position in the scene.
    vec3 intensity(vec4 point, vec4 surface_normal) const {
        // The distance from the light source to the intersection, i.e. the radius of the sphere.
        vec4 shadow_ray = this->pos - point;
        // The proportion of light hitting the surface.
        float prop = dot(normalize(surface_normal), normalize(shadow_ray));
        // Because negative light is not allowed.
        float projection_factor = std::max(prop, 0.0f);

        // The intensity of light is inversely proportional to the distance squared.
        float dist = length(shadow_ray);
        float surface_area_at_radius_r = 4 * M_PI * dist * dist;
        vec3 intensity = this->color / surface_area_at_radius_r;

        return projection_factor * intensity;
    }
};

#endif // POINT_LIGHT_H
