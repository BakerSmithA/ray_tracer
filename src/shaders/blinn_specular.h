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
    float Kd = 0.8; // diffuse weight
    float Ks = 0.3; // specular weight

    BlinnSpecular(vec3 base_color, int specular_exponent = 250): base_color(base_color), specular_exponent(specular_exponent)
    {
        this->diffuse_shader = new Diffuse(base_color);
    }

    // return: the color of the object in ambient lighting conditions, i.e.
    //         with no shadows.
    vec3 ambient_color(vec4 position, const Primitive *prim, const Light &light) const {
        return this->diffuse_shader->ambient_color(position, prim, light) * light.color;
    }

    // return: the color of the intersected surface, as illuminated by a specific light.
    vec3 specular_color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const SpecularLight &light, const int num_shadow_rays) const override {
        //Calculate attenuated light intensity at point
        vec3 intensity = light.intensity(position, prim->compute_normal(position));

        //Calculate reflection ray direction
        vec3 l = normalize(vec3((light.ray_from(position).dir)));
        vec3 v = normalize(vec3((-incoming.dir)));
        vec3 lplusv = vec3(l + v);
        vec3 h = normalize(lplusv); //unit vector

        //Calculate component of h in the direction of the primitive normal
        vec3 surface_normal = normalize(vec3(prim->compute_normal(position)));
        float specular_highlight = dot(h, surface_normal);
        float new_specular_highlight = glm::pow(specular_highlight, specular_exponent);

        /* Calculating Specular Component */
        vec3 specular_component = new_specular_highlight * intensity * vec3(1,1,1);

        /* Calculating Diffuse Component */
        vec3 diffuse_component = diffuse_shader->shadowed_color(position, prim, incoming, scene, light, num_shadow_rays);

        return specular_component * Ks +  diffuse_component * Kd;
    }
};

#endif // SPECULAR_H
