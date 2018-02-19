#include "shader.h"
#include "fresnel.h"
#include "mirror.h"
#include "refraction.h"

#ifndef GLASS_SHADER_H
#define GLASS_SHADER_H

// Models a transparent glass surface.
class Glass: public Shader {
private:
    const Shader *glass_shader;

public:
    Glass(): glass_shader(new Fresnel(new Refraction(0.45), new Mirror(), 0.7f)) {
    }

    // return: the color of the intersected surface, taking shadows from the
    //         light into account.
    vec3 color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light) const override {
        return this->glass_shader->color(position, prim, incoming, scene, light);
    }

    float transparency() const override {
        return this->glass_shader->transparency();
    }
};

#endif // GLASS_SHADER_H
