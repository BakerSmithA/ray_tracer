#include <glm/glm.hpp>
#include "shader.h"

using glm::dot;
using glm::normalize;
using glm::length;

#ifndef DIFFUSE_H
#define DIFFUSE_H

// Models an ideally diffuse surface.
class Diffuse: public Shader {
public:
    // The color of the shader without considering light, refection, etc.
    const vec3 base_color;

    Diffuse(vec3 base_color): base_color(base_color) {
    }

    // return: the color of the intersected surface, as illuminated by a specific light.
    vec3 color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const override {
        optional<Ray> ray = light.ray_from(position);

        // If the light does not have a notion of position, return the ambient
        // color.
        if (!ray.has_value()) {
            return this->base_color * light.color;
        }

        vec4 shadow_ray = ray.value().dir;
        vec4 surface_normal = prim->normal_at(position);
        // The proportion of light hitting the surface.
        float prop = dot(normalize(surface_normal), normalize(shadow_ray));
        // Because negative light is not allowed.
        float projection_factor = std::max(prop, 0.0f);

        return projection_factor * light.intensity(position, surface_normal) * this->base_color;
    }
};

#endif // DIFFUSE_H
