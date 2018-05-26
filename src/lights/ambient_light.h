#pragma once

#include "light.h"

// Models a light which cannot cast shadows, and has the same intensity
// everywhere/
class AmbientLight: public Light {
public:
    AmbientLight(vec3 color): Light(color) {
    }

    // An ambient light has no position, and so its intensity is the same
    // everywhere.
    //
    // param point: the point to be illuminated.
    // param surface_normal: the normal of the object at the point.
    // return: the intensity of each color channel of the light at the given
    //         position in the scene.
    vec3 intensity(vec4 point) const {
        return this->color;
    }

    // An ambient light has no position, and so its projection factor is the
    // same everywhere.
    //
    // param position: the position on the object to get the factor of.
    // param surface_normal: the normal of the illuminated surface.
    // return: the proportion of light which strikes the surface depending
    //         on the angle of the surface to the light.
    virtual float projection_factor(vec4 position, vec4 surface_normal) const {
        return 1.0f;
    }

    // return: nothing since ambient lights do not have a position.
    optional<Ray> ray_from(vec4 point) const {
        return std::nullopt;
    }

    // return: the empty vector because ambient lights cannot cast shadows.
    vector<Ray> random_shadow_rays_from(vec4 point, int num) const {
        return vector<Ray>();
    }
};
