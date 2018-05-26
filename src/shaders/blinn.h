#pragma once

#include "specular.h"

using glm::dot;
using glm::normalize;
using glm::length;

// Models a specular surface.
class Blinn: public Specular {
public:
    Blinn(int specular_exponent = 250): Specular(specular_exponent) {
    }

    // return: the color of the intersected surface, as illuminated by a specific light.
    vec3 specular_color(vec4 position, const Primitive *prim, const vec4 shadow_ray_dir, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const override {
        // Calculate attenuated light intensity at point
        vec3 intensity = light.intensity(position);

        // Calculate reflection ray direction
        vec3 l = normalize(vec3(shadow_ray_dir));
        vec3 v = vec3(-incoming.normalized_dir);
        vec3 lplusv = l + v;
        vec3 h = normalize(lplusv);

        //Calculate component of h in the direction of the primitive normal
        vec4 surface_normal = prim->normal_at(position);
        float specular_highlight = dot(h, vec3(surface_normal));
        float new_specular_highlight = glm::pow(specular_highlight, specular_exponent);

        return new_specular_highlight * intensity;
    }
};
