#include <glm/glm.hpp>

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

    // return: the color of the object in ambient lighting conditions, i.e.
    //         with no shadows.
    vec3 ambient_color(vec4 position, const Primitive *prim, const Light &light) const {
        return this->base_color * light.color;
    }

    // return: the color of the intersected surface, as illuminated by a specific light.
    vec3 specular_color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const SpecularLight &light, const int num_shadow_rays) const override {
        vec4 shadow_ray = light.ray_from(position).dir;
        vec4 surface_normal = prim->compute_normal(position);
        // The proportion of light hitting the surface.
        float prop = dot(normalize(surface_normal), normalize(shadow_ray));
        // Because negative light is not allowed.
        float projection_factor = std::max(prop, 0.0f);

        return projection_factor * light.intensity(position, prim->compute_normal(position)) * this->base_color;
    }
};

#endif // DIFFUSE_H
