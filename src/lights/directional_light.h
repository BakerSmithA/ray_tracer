#include <math.h>
#include "attenuating_light.h"

using glm::vec3;

#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

// Models a point light which radiates light outwards.
class DirectionalLight: public AttenuatingLight {
public:

    vec4 dir;

    DirectionalLight(vec3 color, vec4 pos, vec4 dir): AttenuatingLight(color, pos, Light::LightType::DIFFUSE), dir(dir) {}

    vec3 intensity(vec4 point, vec4 surface_normal, Ray primary) const override {
        //Direction of shadow/incident ray
        vec4 shadow_ray = shadow_ray_to(point).dir;
        // The proportion of light hitting the surface.
        float prop = dot(normalize(surface_normal), normalize(shadow_ray));
        // Because negative light is not allowed.
        float projection_factor = std::max(prop, 0.0f);
        
        vec3 intensity = attenuation_of_light_from(point);

        return projection_factor * intensity;
    }

    // return: a shadow ray that passes through `point` in the direction of the light
    Ray shadow_ray_from(vec4 point) const override {
        // The ray can only be used to check obstructions between the point and
        // light. Therefore it cannot bounce.
        return Ray(point, -dir, 0);
    }
};

#endif // POINT_LIGHT_H
