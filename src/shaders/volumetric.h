#include "shader.h"
#include "../textures/texture.h"

#ifndef VOLUMETRIC_H
#define VOLUMETRIC_H

// Represents a material which light can penetrate, e.g. smoke.
class Volumetric: public Shader {
public:
    // Describes the shape, color, and transparency of the 3d texture.
    Texture<vec4> *texture;

    Volumetric(Texture<vec4> *texture): texture(texture) {
    }

    // return: the color of the volume by performing ray marching though the object.
    vec3 color(const vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const {
        return vec3(0, 1.0, 0);
    }
};

#endif // VOLUMETRIC_H
