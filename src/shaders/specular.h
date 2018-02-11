#include <glm/glm.hpp>
#include "shader.h"

using glm::dot;
using glm::normalize;
using glm::length;

#ifndef SPECULAR_H
#define SPECULAR_H

// Models an ideally diffuse surface.
class Specular: public Shader {
public:
    // The color of specular shader without considering light, refection, etc.
    const vec3 base_color;

    Specular(vec3 base_color): base_color(base_color) {}

    // return: the color of the intersected surface, as illuminated by a specific light.
    vec3 color(vec4 position, const Triangle &tri, const Ray &incoming, const Scene &scene, Light &light) const {
        //A Spectral Texture should only respond to a spectral light
        if(light.type == Light::LightType::SPECULAR){
            return light.intensity(position, tri.normal, incoming) * this->base_color;
        }
        else return vec3(0, 0, 0);
        
    }
};

#endif // SPECULAR_H
