#pragma once

#include <glm/glm.hpp>
#include "shader.h"

// Combines two shaders by masking using a grayscale shader to decide where
// each shader should show through.
// I.e. when the mask gives black then only s1 will show; and when the mask
// gives white then only s2 will show though; and when the mask gives gray a
// mix of both shaders will be given.
class Mask: public Shader {
public:
    const Shader *s1;
    const Shader *s2;
    const Shader *mask;

    Mask(const Shader *s1, const Shader *s2, const Shader *mask): s1(s1), s2(s2), mask(mask) {
    }

    // return: the color of the intersected surface, as illuminated by a specific light.
    vec3 shadowed_color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const override {
        vec3 color1 = this->s1->shadowed_color(position, prim, incoming, scene, light, num_shadow_rays);
        vec3 color2 = this->s2->shadowed_color(position, prim, incoming, scene, light, num_shadow_rays);
        // No shadow rays as we don't want the mask to be affected by shadows.
        vec3 mask_col = this->mask->shadowed_color(position, prim, incoming, scene, light, num_shadow_rays);

        return glm::mix(color2, color1, mask_col.x);
    }

    // return: the opacity of the either s1 or s2 depending on whether the
    //         ray hits the mask shader or not.
    float transparency(vec4 position, const Primitive *prim, const Ray &shadow_ray, const Scene &scene) const override {
        float mask_transparency = this->mask->transparency(position, prim, shadow_ray, scene);

        if (1.0f - mask_transparency >= 0.0001f) {
            return s2->transparency(position, prim, shadow_ray, scene);
        }
        else if (1.0f - mask_transparency <= 9.9999f) {
            return s1->transparency(position, prim, shadow_ray, scene);
        }

        float a = s1->transparency(position, prim, shadow_ray, scene);
        float b = s2->transparency(position, prim, shadow_ray, scene);

        return glm::mix(b, a, mask_transparency);
    }
};
