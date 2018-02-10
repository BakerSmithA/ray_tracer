#include <math.h>
#include "point_light.h"

using glm::vec3;
using glm::dot;


#ifndef BLINN_POINT_LIGHT_H
#define BLINN_POINT_LIGHT_H

// Models a point light which radiates light outwards.
class BlinnPointLight: public PointLight {
public:

    BlinnPointLight(vec3 color, vec4 pos, float dropoff): PointLight(color, pos, dropoff) {}

    vec3 intensity(vec4 point, vec4 surface_normal, Ray primary) const {

        //Calculate reflection ray direction
        vec4 incident_dir = shadow_ray_to(point).dir;
        vec4 l = -incident_dir;
        vec4 v = primary.dir;
        vec4 lplusv = l + v;
        vec4 h = lplusv / length(lplusv); //unit vector
        
        //Calculate component of h in the direction of the normal
        float proj_prop = dot(h, glm::normalize(surface_normal));
        float proj_prop_scaled_down = glm::pow(proj_prop, dropoff);

        vec3 intensity = attenuation_of_light_from(point);

        return proj_prop_scaled_down * intensity;
    }
    
};

#endif // BLINN_POINT_LIGHT_H


