#include <glm/glm.hpp>
#include "shader.h"
#include "diffuse.h"

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
    int specular_exponent;
    Diffuse* diffuse_shader;
    float Kd = 0.8; // phong model diffuse weight 
    float Ks = 0.2; // phong model specular weight 

    BlinnSpecular(vec3 base_color, int specular_exponent = 10): base_color(base_color), specular_exponent(specular_exponent) 
    {
        this->diffuse_shader = new Diffuse(base_color);
    }

    // return: the color of the intersected surface, as illuminated by a specific light.
    vec3 color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const PointLight &light) const override {
        //Calculate attenuated light intensity at point
        vec3 intensity = light.intensity(position, prim->compute_normal(position));

        
        //Calculate reflection ray direction
        vec4 l = light.shadow_ray_to(position).dir;
        vec4 v = -incoming.dir;
        vec3 lplusv = vec3(l + v);
        vec3 h = normalize(lplusv); //unit vector

        //Calculate component of h in the direction of the primitive normal
        vec3 surface_normal = vec3(prim->compute_normal(position));
        float specular_highlight = dot(h, surface_normal);
        float new_specular_highlight = glm::pow(specular_highlight, specular_exponent);

        /* Calculating Specular Component */
        vec3 specular_component = new_specular_highlight * intensity * this->base_color;

        /* Calculating Diffuse Component */
        vec3 diffuse_component = vec3(0,0,0); //diffuse_shader->color(position, prim, incoming, scene, light);

        return specular_component * Ks +  diffuse_component * Kd;
    }
};

#endif // SPECULAR_H