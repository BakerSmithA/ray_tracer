#pragma once

#include "shader.h"

// Surface which outputs the same color irrespective of lighting conditions,
// therefore the surface does not respond to shadows either.
class FlatColor: public Shader {
public:
    const vec3 base_color;
    const float alpha;

    FlatColor(vec3 base_color, float alpha): base_color(base_color), alpha(alpha) {
    }

    // return: the base color of the surface irrespective of lighting or whether
    //         other objects are occluding the light.
    vec3 shadowed_color(const vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const override {
        return this->base_color;
    }

    float transparency(vec4 position, const Primitive *prim, const Ray &shadow_ray, const Scene &scene) const {
        return this->alpha;
    }
};
