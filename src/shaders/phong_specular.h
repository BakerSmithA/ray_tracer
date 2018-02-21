#include <glm/glm.hpp>
#include "shader.h"
#include "diffuse.h"

using glm::dot;
using glm::normalize;
using glm::length;

#ifndef PHONG_SPECULAR_H
#define PHONG_SPECULAR_H

// Models an ideally diffuse surface.
class PhongSpecular: public Shader {
public:
    const vec3 base_color;
    int specular_exponent;
    Diffuse* diffuse_shader;
    float Kd = 0.8; // diffuse weight 
    float Ks = 0.2; // specular weight 

    PhongSpecular(vec3 base_color, int specular_exponent = 250): base_color(base_color), specular_exponent(specular_exponent) 
    {
        this->diffuse_shader = new Diffuse(base_color);
    }


    // return: the color of the intersected surface, as illuminated by a specific light.
    vec3 color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const PointLight &light) const override {
         
         //Calculate reflection ray direction
        vec3 l = vec3(light.shadow_ray_to(position).dir);
        vec3 surface_normal = normalize(vec3(prim->compute_normal(position)));
        vec3 reflected_dir = normalize(2.0f * dot(l, surface_normal) * surface_normal - l);

        //Viewing direction
        vec3 v = normalize(-vec3(incoming.dir));

        //Calculate component of viewing direction in the direction of reflected ray
        float specular_highlight = dot(v, reflected_dir);
        float new_specular_highlight = glm::pow(specular_highlight, specular_exponent);

        vec3 intensity = light.intensity(position, prim->compute_normal(position));

        /* Calculating Specular Component */
        vec3 specular_component = new_specular_highlight * intensity * vec3(1,1,1);

        /* Calculating Diffuse Component */
        vec3 diffuse_component = diffuse_shader->color(position, prim, incoming, scene, light);

        return specular_component * Ks +  diffuse_component * Kd;
    }
};

#endif // SPECULAR_H
