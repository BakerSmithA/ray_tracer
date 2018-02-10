#include "light.h"

#ifndef ATTENUATING_LIGHT_H
#define ATTENUATING_LIGHT_H

using glm::vec3;
using glm::length;

class AttenuatingLight : public Light {
public:

    //All lights that reduce in brightness with distance (attenuating lights) have a position 
    vec3 pos;

    AttenuatingLight(vec3 color, vec3 pos) : Light(color), pos(pos) {}

    //Attenuating lights with directions (create shadow/incident rays) always cast shadows
    virtual bool does_cast_shadows() const override { return true; }

    //The direction of the shadow ray is based on implementation
    //Can either be calculated 
    virtual Ray shadow_ray_to(vec4 point) const = 0;

    // The intensity of light is inversely proportional to the distance squared.
    vec3 attenuation_of_light_from(vec3 point) const {
        float dist = length(point - pos);
        float surface_area_at_radius_r = 4 * M_PI * dist * dist;
        vec3 intensity = this->color / surface_area_at_radius_r;
        return intensity;
    }
}

#endif // ATTENUATING_LIGHT_H

