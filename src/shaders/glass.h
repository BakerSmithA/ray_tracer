#include "shader.h"
#include "fresnel.h"
#include "mirror.h"
#include "refraction.h"
#include "blinn_specular.h"
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
        Shader *refraction = new Refraction(0.45);
        Shader *mirror = new Mirror();
        Shader *transparent = new Fresnel(refraction, mirror, 0.7f);

        // ==== Lighting ===
        // Specular is used to add a bright spot.
        Shader *specular = new BlinnSpecular(vec3(0, 0, 0), 250);
        // Clamp the specular to make the bright spot slightly less bright,
        // since we are adding the specular.
        Shader *scaled_spec = new ScaleShader(specular, 0.0f, 0.6f);

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

    // return: the color of the object in ambient lighting conditions, i.e.
    //         with no shadows.
    vec3 ambient_color(vec4 position, const Primitive *prim, const AmbientLight &light) const {
        return light.color;
    }

    // return: the color of the intersected surface, taking shadows from the
    //         light into account.
    vec3 specular_color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const SpecularLight &light, const int num_shadow_rays) const override {
        return this->glass_shader->specular_color(position, prim, incoming, scene, light, num_shadow_rays);
    }

    float transparency() const override {
        return 0.7f;
    }
};

#endif // GLASS_SHADER_H
