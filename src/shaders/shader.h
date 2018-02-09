#include "../geometry/scene.h"

#ifndef SHADER_H
#define SHADER_H

// Used to model different types of surfaces, e.g. matte, glossy, etc.
class Shader {
public:
    // The color of the shader without considering light, refection, etc.
    const vec3 base_color;

    Shader(vec3 base_color): base_color(base_color) {
    }

    // return: the color of the intersected surface, as illuminated by a specific light.
    virtual vec3 color(vec4 position, vec4 surface_normal, const Ray &incoming, const Scene &scene, const PointLight &light) const = 0;
};

#endif // SHADER_H
