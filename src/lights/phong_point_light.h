#include <math.h>
#include "point_light.h"

using glm::vec3;
using glm::dot;

#ifndef PHONG_POINT_LIGHT_H
#define PHONG_POINT_LIGHT_H

// Models a point light which radiates light outwards.
class PhongPointLight: public PointLight {
public:

    PhongPointLight(vec3 color, vec4 pos, float dropoff): PointLight(color, pos, dropoff) {}

    vec3 intensity(vec4 point, vec4 surface_normal, Ray primary) const {

        //Calculate reflection ray direction
        vec4 incident_dir = shadow_ray_to(point).dir;
        vec4 L = -incident_dir;
        vec4 reflected_dir = 2.0f * dot(L, surface_normal) * surface_normal - L;
        
        //Calculate component of viewing direction in the direction of reflected ray
        float proj_prop = dot(normalize(primary.dir), normalize(reflected_dir));
        float proj_prop_scaled_down = glm::pow(proj_prop, dropoff);

        vec3 intensity = attenuation_of_light_from(point);

        return proj_prop_scaled_down * intensity;
    }

};

#endif // PHONG_POINT_LIGHT_H
