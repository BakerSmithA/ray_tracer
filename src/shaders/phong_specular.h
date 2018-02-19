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

    PhongSpecular(vec3 base_color, int dropoff = 3): base_color(base_color), dropoff(dropoff) 
    {
    }

    // return: the color of the intersected surface, as illuminated by a specific light.
    vec3 color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light) const override {
         
        //  //Calculate reflection ray direction
        // vec4 l = light.shadow_ray_to(position).dir;
        // vec4 v = -incoming.dir;
        // vec4 lplusv = l + v;
        // vec4 h = normalize(lplusv); //unit vector

        // //Calculate component of h in the direction of the normal
        // float proj_prop = dot(h, prim->compute_normal(position));
        // float proj_prop_scaled_down = glm::pow(proj_prop, dropoff);


        // //Calculate reflection ray direction
        // vec4 surface_normal = prim->compute_normal(position)
        // vec4 reflected_dir = 2.0f * dot(l, surface_normal) * surface_normal - l;
        
        // //Calculate component of viewing direction in the direction of reflected ray
        // vec4 v = -primary.dir;
        // float proj_prop = dot(normalize(v), normalize(reflected_dir));
        // float proj_prop_scaled_down = glm::pow(proj_prop, dropoff);

        // vec3 intensity = light.intensity(position, prim->compute_normal(position));

        // return proj_prop_scaled_down * intensity * this->base_color;
        return this->base_color;
    }
};

#endif // SPECULAR_H
