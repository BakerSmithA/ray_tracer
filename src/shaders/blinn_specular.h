#include <glm/glm.hpp>
#include "shader.h"

using glm::dot;
using glm::normalize;
using glm::length;

#ifndef BLINN_SPECULAR_H
#define BLINN_SPECULAR_H

// Models an ideally diffuse surface.
class BlinnSpecular: public Shader {
public:
    // The color of the shader without considering light, refection, etc.
    const vec3 base_color;
    int dropoff;

    BlinnSpecular(vec3 base_color, int dropoff = 3): base_color(base_color), dropoff(dropoff) 
    {
    }

    // return: the color of the intersected surface, as illuminated by a specific light.
    vec3 color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const PointLight &light) const override {
         
         //Calculate reflection ray direction
        vec4 l = light.shadow_ray_to(position).dir;
        vec4 v = -incoming.dir;
        vec3 lplusv = vec3(l + v);
        vec3 h = normalize(lplusv); //unit vector

        //Calculate component of h in the direction of the primitive normal
        vec3 surface_normal = vec3(prim->compute_normal(position));
        float proj_prop = dot(h, surface_normal);
        float proj_prop_scaled_down = glm::pow(proj_prop, dropoff);

        vec3 intensity = light.intensity(position, prim->compute_normal(position));

        return proj_prop_scaled_down * intensity * this->base_color;
    }
};

#endif // SPECULAR_H
