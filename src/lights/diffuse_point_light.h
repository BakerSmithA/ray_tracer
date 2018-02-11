#include <math.h>
#include "point_light.h"

using glm::vec3;

#ifndef DIFFUSE_POINT_LIGHT_H
#define DIFFUSE_POINT_LIGHT_H

// Models a point light which radiates light outwards.
class DiffusePointLight: public PointLight {
public:

    DiffusePointLight(vec3 color, vec4 pos): PointLight(color, pos, Light::LightType::DIFFUSE){}

    vec3 intensity(vec4 point, vec4 surface_normal, Ray primary) const override {
        //Direction of shadow/incident ray
        vec4 shadow_ray = shadow_ray_from(point).dir;
        // The proportion of light hitting the surface.
        float prop = dot(normalize(surface_normal), normalize(shadow_ray));
        // Because negative light is not allowed.
        float projection_factor = std::max(prop, 0.0f);
        
        vec3 intensity = attenuation_of_light_from(point);

        return projection_factor * intensity;
    }

};

#endif // DIFFUSE_POINT_LIGHT_H
