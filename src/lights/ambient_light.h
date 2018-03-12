#include "light.h"

#ifndef AMBIENT_LIGHT_H
#define AMBIENT_LIGHT_H

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
    virtual vec3 intensity(vec4 point, vec4 surface_normal) const {
        return this->color;
    }

    // return: nothing since ambient lights do not have a position.
    virtual optional<Ray> ray_from(vec4 point) const {
        return std::nullopt;
    }

    // return: the empty vector because ambient lights cannot cast shadows.
    virtual vector<Ray> random_shadow_rays_from(vec4 point, int num) const {
        return vector<Ray>();
    }
};

#endif // AMBIENT_LIGHT_H
