#include <glm/glm.hpp>
#include "shader.h"

using glm::dot;
using glm::normalize;
using glm::length;

#ifndef DIFFUSE_H
#define DIFFUSE_H

// Models an ideally diffuse surface.
class Diffuse: public ShadowedShader {
public:
    // The color of the shader without considering light, refection, etc.
    const vec3 base_color;

    Diffuse(vec3 base_color): base_color(base_color) {
    }

    // return: the color of the intersected surface, as illuminated by a specific light.
    vec3 color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const override {
        vec4 normal = prim->normal_at(position);
        return light.projection_factor(position, normal) * light.intensity(position) * this->base_color;
    }
};

#endif // DIFFUSE_H
