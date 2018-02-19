#include <glm/glm.hpp>
#include "shader.h"

using glm::dot;
using glm::normalize;
using glm::length;

#ifndef PHONG_SPECULAR_H
#define PHONG_SPECULAR_H

// Models an ideally diffuse surface.
class PhongSpecular: public Shader {
public:
    // The color of the shader without considering light, refection, etc.
    const vec3 base_color;
    int dropoff;

    PhongSpecular(vec3 base_color, int dropoff = 10): base_color(base_color), dropoff(dropoff) 
    {
    }

    // return: the color of the intersected surface, as illuminated by a specific light.
    vec3 color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const PointLight &light) const override {
         
         //Calculate reflection ray direction
        vec3 l = -vec3(light.shadow_ray_to(position).dir);
        vec3 surface_normal = normalize(vec3(prim->compute_normal(position)));
        vec3 reflected_dir = 2.0f * dot(l, surface_normal) * surface_normal - l;

        //Viewing direction
        vec3 v = -vec3(incoming.dir);

        //Calculate component of viewing direction in the direction of reflected ray
        float proj_prop = dot(v, reflected_dir);
        float proj_prop_scaled_down = glm::pow(proj_prop, dropoff);

        vec3 intensity = light.intensity(position, prim->compute_normal(position));

        return proj_prop_scaled_down * intensity * this->base_color;
    }
};

#endif // SPECULAR_H
