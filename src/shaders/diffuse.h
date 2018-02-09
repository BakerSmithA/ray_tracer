#include <glm/glm.hpp>

using glm::dot;
using glm::normalize;
using glm::length;

#ifndef DIFFUSE_H
#define DIFFUSE_H

// Models an ideally diffuse surface.
class Diffuse: public Shader {
public:
    Diffuse(vec3 base_color): Shader(base_color) {
    }

    // return: the color of the intersected surface, as illuminated by a specific light.
    vec3 color(const Intersection &intersection, const Ray &incoming, const Scene &scene, const PointLight &light) const {
        // The distance from the light source to the intersection, i.e. the radius of the sphere.
        vec4 shadow_ray = light.pos - intersection.pos;
        // The proportion of light hitting the surface.
        float prop = dot(normalize(intersection.triangle.normal), normalize(shadow_ray));
        // Because negative light is not allowed.
        float projection_factor = std::max(prop, 0.0f);

        return projection_factor * light.intensity(length(shadow_ray)) * this->base_color;
    }
};

#endif // DIFFUSE_H
