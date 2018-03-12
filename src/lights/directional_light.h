#include "shadow_light.h"

#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

class DirectionalLight: public ShadowLight {
public:
    // The direction of the parallel light rays.
    const vec4 dir;
    // Used to create soft shadows. There is no physical radius of the light,
    // however there is a radius to the sphere to which shadow rays rays will
    // be created.
    const float radius;

    DirectionalLight(vec3 color, vec4 dir, float radius):
        ShadowLight(color), dir(dir), radius(radius) {
    }

    // The intensity of a directional light depends only on the angle of the
    // light to the surface normal. It is assumed the light is infinitely far
    // away, hence why the light rays are parallel.
    //
    // param point: the point to be illuminated.
    // param surface_normal: the normal of the object at the point.
    // return: the intensity of each color channel of the light at the given
    //         position in the scene.
    virtual vec3 intensity(vec4 point, vec4 surface_normal) const {
        return glm::dot(dir, surface_normal) * this->color;
    }

    // The shadow ray depends only on the direction of the light source, since
    // it has no position.
    //
    // return: a shadow ray from the point and the light source. Or, returns
    //         nothing if the light does not cast shadows.
    virtual Ray shadow_ray_from(vec4 point) const {
        // TODO
        throw std::runtime_error("not implemented");
    }

    // return: a number of randomly selected shadow rays from an area around
    //         the light to the point. Or, returns nothing if the light does
    //         not cast shadows.
    virtual vector<Ray> random_shadow_rays_from(vec4 point, int num) const {
        // TODO
        throw std::runtime_error("not implemented");
    }
};

#endif // DIRECTIONAL_LIGHT_H
