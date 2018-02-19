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

    // return: the base color of the surface irrespective of lighting.
    vec3 color(const vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light) const {
        return this->base_color;
    }

    vec3 shadowed_color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light) const {
        return this->color(position, prim, incoming, scene, light);
    }
};

#endif // COLOR_SHADER_H
