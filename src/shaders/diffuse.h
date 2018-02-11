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

    // return: the color of the intersected surface, as illuminated by a specific light.
    vec3 color(vec4 position, const Triangle &tri, const Ray &incoming, const Scene &scene, const Light &light) const override {
        return light.intensity(position, tri.normal) * this->base_color;
    }

    bool is_transparent() const override {
        return false;
    }
};

#endif // DIFFUSE_H
