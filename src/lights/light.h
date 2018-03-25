#include <optional>

using std::optional;

#ifndef LIGHT_H
#define LIGHT_H

// Models a light source.
class Light {
public:
    const vec3 color;

    Light(vec3 color): color(color) {
    }

    virtual ~Light() {
    }

    // param point: the point to be illuminated.
    // return: the intensity of each color channel of the light at the given
    //         position in the scene.
    virtual vec3 intensity(vec4 point) const = 0;

    // param position: the position on the object to get the factor of.
    // param surface_normal: the normal of the illuminated surface.
    // return: the proportion of light which strikes the surface depending
    //         on the angle of the surface to the light.
    virtual float projection_factor(vec4 position, vec4 surface_normal) const = 0;

    // return: a ray from the point and the light source. Or, nothing if the
    //         light cannot cast rays, e.g. ambient lighting.
    virtual optional<Ray> ray_from(vec4 point) const = 0;

    // return: a number of randomly selected shadow rays from an area around
    //         the light to the point. Or, the empty vector if the light
    //         cannot cast shadows, e.g. ambient lighting.
    virtual vector<Ray> random_shadow_rays_from(vec4 point, int num) const = 0;
};

#endif // LIGHT_H
