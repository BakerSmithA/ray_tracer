#include <glm/glm.hpp>
#include "shader.h"

using glm::dot;
using glm::normalize;
using glm::length;

#ifndef DIFFUSE_H
#define DIFFUSE_H

// Models an ideally diffuse surface.
class Diffuse: public Shader {
public:
    // The color of the shader without considering light, refection, etc.
    const vec3 base_color;

    Diffuse(vec3 base_color): base_color(base_color) {}

    // return: the color of the intersected surface, as illuminated by a specific light.
    vec3 color(vec4 position, const Triangle &tri, const Ray &incoming, const Scene &scene, Light &light) const {
        //A Diffuse Texture should only respond to a diffuse light
        if(light.type == Light::LightType::DIFFUSE){
            return light.intensity(position, tri.normal, incoming) * this->base_color;
        }
        else return vec3(0, 0, 0);
        
    }
};

#endif // DIFFUSE_H
