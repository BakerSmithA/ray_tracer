#include "shader.h"
#include "fresnel.h"
#include "mirror.h"
#include "refraction.h"
#include "blinn.h"
#include "mix.h"
#include "flat_color.h"
#include "scale.h"

#ifndef GLASS_SHADER_H
#define GLASS_SHADER_H

// Models a transparent glass surface.
class Glass: public Shader {
private:
    Shader *glass_shader;

public:
    Glass() {
        // ==== Material ====
        const float refractive_index = 2.0f;

        Shader *refraction = new Refraction(refractive_index);
        Shader *mirror = new Mirror();
        Shader *transparent = new Fresnel(refraction, mirror, refractive_index, 0.7f);

        // ==== Lighting ===
        // Specular is used to add a bright spot.
        Shader *specular = new Blinn(250);
        // Clamp the specular to make the bright spot slightly less bright,
        // since we are adding the specular.
        Shader *scaled_spec = new ScaleShader(specular, 0.0f, 0.2f);

        // A small amount of diffuse light is used to apply shading. This
        // allows the viewer to more easily tell the shape of the object.
        Shader *diffuse = new Diffuse(vec3(1, 1, 1));
        // Scale the lighting of the diffuse to be between a min and max
        // to stop the glass from appearing opaque, as we are multiplying
        // by the diffuse shader.
        Shader *scaled_diffuse = new ScaleShader(diffuse, 0.7f, 1.0f);

        // ==== Mixing ====
        Shader *mix_diffuse = Mix::multiply(transparent, scaled_diffuse);
        Shader *mix_specular = Mix::add(scaled_spec, mix_diffuse);

        this->glass_shader = mix_specular;
    }

    // return: the color of the intersected surface, taking shadows from the
    //         light into account.
    vec3 color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const override {
        return this->glass_shader->shadowed_color(position, prim, incoming, scene, light, num_shadow_rays);
    }

    float transparency() const override {
        return 0.7f;
    }
};

#endif // GLASS_SHADER_H
