#pragma once

#include "ray_spawner.h"

// Shader which performs no computation and gets the color of the object behind.
class Id: public RaySpawner {
public:
    // param position: the position of the intersection of the prim with the shadow ray.
    // param prim: the primitive to calculate the transparency of.
    // param shadow_ray: the shadow ray from the original object the shadow is
    //                   being tested for, to the light source.
    // return: the proportion by which light is let through the
    //         material. E.g. a value of 1 is totally transparent, and a value
    //         of 0 is totally opaque.
    float transparency(vec4 position, const Primitive *prim, const Ray &shadow_ray, const Scene &scene) const {
        return 0.0f;
    }

    // return: the ray direction used to find the color of the shader, e.g. the
    //         reflected ray for a mirror.
    vec4 outgoing_ray_dir(const vec4 position, const Primitive *prim, const Ray &incoming) const {
        return incoming.dir;
    }
};
