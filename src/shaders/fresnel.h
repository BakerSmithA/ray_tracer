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

    Fresnel(const Shader *s1, const Shader *s2): s1(s1), s2(s2) {
    }

    // return: the color of the intersected surface, as illuminated by a specific light.
    vec3 color(vec4 position, const Triangle &tri, const Ray &incoming, const Scene &scene, const Light &light) const override {
        vec4 normal = normalize(tri.normal);
        vec4 incoming_dir = normalize(incoming.dir);
        float kr;
        float cosi = dot(normal, incoming_dir);
        float etai = 1, etat = 1.3; //ior. TODO: Change this value for different materials?
        if (cosi > 0) { std::swap(etai, etat); }
        // Compute sini using Snell's law
        float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
        // Total internal reflection
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

        vec3 color1 = this->s1->color(position, tri, incoming, scene, light);
        vec3 color2 = this->s2->color(position, tri, incoming, scene, light);

        return mix(color1, color2, kr);
    }

    bool is_transparent() const override {
        return this->s1->is_transparent() || this->s2->is_transparent();
    }
};

#endif // FRESNEL_H
