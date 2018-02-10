#include <glm/glm.hpp>

using glm::dot;
using glm::normalize;
using glm::length;

#ifndef DIFFUSE_H
#define DIFFUSE_H

// Models an ideally diffuse surface.
class Diffuse: public Shader {
public:
    // Inherit the default constructor.
    using Shader::Shader;

    // return: the color of the intersected surface, as illuminated by a specific light.
    vec3 color(vec4 position, vec4 surface_normal, const Ray &incoming, const Scene &scene, const Light &light) const {
        return light.intensity(position, surface_normal) * this->base_color;
    }
};

#endif // DIFFUSE_H
