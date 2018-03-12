#include "shader.h"

#ifndef COLOR_SHADER_H
#define COLOR_SHADER_H

// Surface which outputs the same color irrespective of lighting conditions,
// therefore the surface does not respond to shadows either.
class FlatColor: public Shader {
public:
    const vec3 base_color;

    FlatColor(vec3 base_color): base_color(base_color) {
    }

    // return: the color of the object in ambient lighting conditions, i.e.
    //         with no shadows.
    vec3 ambient_color(vec4 position, const Primitive *prim, const Light &light) const {
        return this->base_color;
    }

    // return: the base color of the surface irrespective of lighting.
    vec3 specular_color(const vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const SpecularLight &light, const int num_shadow_rays) const override {
        return this->base_color;
    }
};

#endif // COLOR_SHADER_H
