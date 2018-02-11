#include <math.h>
#include "attenuating_light.h"

using glm::vec3;

#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

// Models a point light which radiates light outwards.
class PointLight: public AttenuatingLight {
public:

    PointLight(vec3 color, vec4 pos, Light::LightType type): AttenuatingLight(color, pos, type) {}

    // return: a shadow ray that passes through `point` in the direction of the light
    virtual Ray shadow_ray_from(vec4 point) const override {
        // The ray can only be used to check obstructions between the point and
        // light. Therefore it cannot bounce.
        return Ray(point, this->pos - point, 0);
    }
};

#endif // POINT_LIGHT_H
