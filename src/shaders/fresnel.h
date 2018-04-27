#include <glm/glm.hpp>

using glm::dot;
using glm::normalize;
using glm::mix;

#ifndef FRESNEL_H
#define FRESNEL_H

// Used to combine two shaders with a fresnel effect. Shader s1 will be shown
// more the closer the angle of incidence is to the normal.
class Fresnel: public Shader {
public:
    const Shader *s1;
    const Shader *s2;
    // The ratio n1/n2, assuming n1 is a vacuum. This indicates how much light
    // slows down when entering the medium.
    const float ray_velocity_ratio;
    const float base_transparency;

    Fresnel(const Shader *s1, const Shader *s2, float ray_velocity_ratio, float base_transparency):
        s1(s1), s2(s2), ray_velocity_ratio(ray_velocity_ratio), base_transparency(base_transparency) {
    }

    // return: the color of the intersected surface, as illuminated by a specific light.
    vec3 shadowed_color(const vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const override {
        vec3 normal_3d = normalize(vec3(prim->normal_at(position)));
        vec3 incoming_3d = vec3(incoming.normalized_dir);

        // From Scratch a Pixel:
        float cosi = glm::clamp(-1.0f, 1.0f, dot(normal_3d, incoming_3d));

        float etai = 1, etat = this->ray_velocity_ratio;
        if (cosi > 0) { std::swap(etai, etat); }
        // Compute sini using Snell's law
        float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
        // Total internal reflection
        float kr;
        if (sint >= 1) {
            kr = 1;
        }
        else {
            float cost = sqrtf(std::max(0.f, 1 - sint * sint));
            cosi = fabsf(cosi);
            float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
            float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
            kr = (Rs * Rs + Rp * Rp) / 2;
        }

        vec3 color1 = this->s1->shadowed_color(position, prim, incoming, scene, light, num_shadow_rays);
        vec3 color2 = this->s2->shadowed_color(position, prim, incoming, scene, light, num_shadow_rays);

        return mix(color1, color2, kr);
    }

    float transparency(vec4 position, const Primitive *prim, const Ray &shadow_ray, const Scene &scene) const override {
        return this->base_transparency;
    }
};

#endif // FRESNEL_H
