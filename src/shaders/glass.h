#include "shader.h"
#include "fresnel.h"
#include "mirror.h"
#include "refraction.h"
#include "blinn_specular.h"
#include "mix.h"

#ifndef GLASS_SHADER_H
#define GLASS_SHADER_H

// Models a transparent glass surface.
class Glass: public Shader {
private:
    Shader *glass_shader;

public:
    Glass() {
        // TODO: Clean up this
        Shader *refraction = new Refraction(0.45);
        Shader *mirror = new Mirror();
        Shader *transparent = new Fresnel(refraction, mirror, 0.7f);
        Shader *specular = new BlinnSpecular(vec3(0, 0, 0), 250);
        Shader *add = Mix::ratio(specular, transparent, 0.7);
        this->glass_shader = add;
    }

    // return: the color of the intersected surface, taking shadows from the
    //         light into account.
    vec3 color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const PointLight &light, const int num_shadow_rays) const override {
        return this->glass_shader->color(position, prim, incoming, scene, light, num_shadow_rays);
    }

    float transparency() const override {
        return this->glass_shader->transparency();
    }
};

#endif // GLASS_SHADER_H
