#include <glm/glm.hpp>

using glm::mix;

#ifndef MIX_SHADER_H
#define MIX_SHADER_H

// Mixes two shaders together with given ratio.
class Mix: public Shader {
public:
    const Shader *s1;
    const Shader *s2;
    // The proportion to show of s2. The proportion to show of s1 is 1-proportion.
    const float proportion;

    Mix(const Shader *s1, const Shader *s2, const float proportion):
        s1(s1), s2(s2), proportion(proportion)
    {
    }

    // return: the color of the intersected surface, as illuminated by a specific light.
    vec3 color(vec4 position, const Primitive &prim, const Ray &incoming, const Scene &scene, const Light &light) const override {
        vec3 color1 = this->s1->color(position, prim, incoming, scene, light);
        vec3 color2 = this->s2->color(position, prim, incoming, scene, light);

        return mix(color1, color2, this->proportion);
    }

    // return: the opacity of each shader mixed in the specified proportion.
    float opacity() const override {
        return mix(s1->opacity(), s2->opacity(), this->proportion);
    }
};

#endif // MIX_SHADER_H
