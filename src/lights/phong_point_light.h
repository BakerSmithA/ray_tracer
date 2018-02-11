#include <math.h>
#include "point_light.h"

using glm::vec3;
using glm::dot;

#ifndef PHONG_POINT_LIGHT_H
#define PHONG_POINT_LIGHT_H

// Models a point light which radiates light outwards.
class PhongPointLight: public PointLight {
public:

    float dropoff;
    PhongPointLight(vec3 color, vec4 pos, float dropoff): PointLight(color, pos, Light::LightType::SPECULAR), dropoff(dropoff) {}

    vec3 intensity(vec4 point, vec4 surface_normal, Ray primary) const {

        //Calculate reflection ray direction
        vec4 l = shadow_ray_from(point).dir;
        vec4 reflected_dir = 2.0f * dot(l, surface_normal) * surface_normal - l;
        
        //Calculate component of viewing direction in the direction of reflected ray
        vec4 v = -primary.dir;
        float proj_prop = dot(normalize(v), normalize(reflected_dir));
        float proj_prop_scaled_down = glm::pow(proj_prop, dropoff);

        vec3 intensity = attenuation_of_light_from(point);

        return proj_prop_scaled_down * intensity;
    }

};

#endif // PHONG_POINT_LIGHT_H
