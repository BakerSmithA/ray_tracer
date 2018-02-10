#include <math.h>
#include "attenuating_light.h"

using glm::vec3;

#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

// Models a point light which radiates light outwards.
class PointLight: public AttenuatingLight {
public:

    float dropoff;
    PointLight(vec3 color, vec4 pos, float dropoff): AttenuatingLight(color, pos), dropoff(dropoff) {}

    // return: a shadow ray that passes through `point` in the direction of the light
    virtual Ray shadow_ray_to(vec4 point) const override {
        // The ray can only be used to check obstructions between the point and
        // light. Therefore it cannot bounce.
        return Ray(point, point - pos, 0);
    }
};

#endif // POINT_LIGHT_H
