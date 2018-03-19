#include "shader.h"

#ifndef SPECULAR_H
#define SPECULAR_H

// Models specular a surface which has only specular highlights. The base color
// of the material is black.
class Specular: public Shader {
public:
    // The higher the number, the larger the bright-spot.
    const int specular_exponent;

    Specular(int specular_exponent = 250): specular_exponent(specular_exponent) {
    }

    // return: the color of the object given the normalised indicent ray.
    virtual vec3 specular_color(vec4 position, const Primitive *prim, const vec4 shadow_ray_dir, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const = 0;

    vec3 color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const override {
        optional<Ray> shadow_ray = light.ray_from(position);

        // If the light does not have a notion of position, return black.
        if (!shadow_ray.has_value()) {
            return vec3(0.0f, 0.0f, 0.0f);
        }

        vec4 shadow_ray_dir = shadow_ray.value().normalized_dir;
        return this->specular_color(position, prim, shadow_ray_dir, incoming, scene, light, num_shadow_rays);
    }
};

#endif // SPECULAR_H
