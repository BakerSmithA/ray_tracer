#include <glm/glm.hpp>
#include "shader.h"

using glm::dot;
using glm::normalize;
using glm::length;

#ifndef AMBIENT_H
#define AMBIENT_H

// Models an ideally diffuse surface.
class Ambient: public Shader {
public:
    // The color of ambient shader without considering light, refection, etc.
    const vec3 base_color;

    Ambient(vec3 base_color): base_color(base_color) {}

    // return: the color of the intersected surface, as illuminated by a specific light.
    vec3 color(vec4 position, const Triangle &tri, const Ray &incoming, const Scene &scene, Light &light) const {
        //A Ambient Texture should only respond to a ambient light
        if(light.type == Light::LightType::AMBIENT){
            return light.intensity(position, tri.normal, incoming) * this->base_color;
        }
        else return vec3(0, 0, 0);
        
    }
};

#endif // AMBIENT_H
