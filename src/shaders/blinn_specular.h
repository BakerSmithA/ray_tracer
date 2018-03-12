#include <glm/glm.hpp>
#include "specular.h"

using glm::dot;
using glm::normalize;
using glm::length;

#ifndef BLINN_H
#define BLINN_H

// Models a specular surface.
class Blinn: public Specular {
public:
    Blinn(int specular_exponent = 250): Specular(specular_exponent) {
    }

    // return: the color of the intersected surface, as illuminated by a specific light.
    vec3 specular_color(vec4 position, const Primitive *prim, const vec4 shadow_ray_dir, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const override {
        // Calculate attenuated light intensity at point
        vec3 intensity = light.intensity(position, prim->compute_normal(position));

        // Calculate reflection ray direction
        vec3 l = normalize(vec3((shadow_ray_dir)));
        vec3 v = normalize(vec3((-incoming.dir)));
        vec3 lplusv = vec3(l + v);
        vec3 h = normalize(lplusv);

        //Calculate component of h in the direction of the primitive normal
        vec3 surface_normal = normalize(vec3(prim->compute_normal(position)));
        float specular_highlight = dot(h, surface_normal);
        float new_specular_highlight = glm::pow(specular_highlight, specular_exponent);

        return new_specular_highlight * intensity;
    }
};

#endif // BLINN_H
