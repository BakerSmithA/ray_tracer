#include <glm/glm.hpp>
#include <functional>

using std::function;

#ifndef MIX_SHADER_H
#define MIX_SHADER_H

// Mixes two shaders together with given ratio.
class Mix: public Shader {
public:
    const Shader *s1;
    const Shader *s2;
    const function<vec3(vec3, vec3)> combine_colors;

    Mix(const Shader *s1, const Shader *s2, function<vec3(vec3, vec3)> combine_colors):
        s1(s1), s2(s2), combine_colors(combine_colors)
    {
    }

    // return: the color of the intersected surface, as illuminated by a specific light.
    vec3 shadowed_color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const override {
        vec3 color1 = this->s1->shadowed_color(position, prim, incoming, scene, light, num_shadow_rays);
        vec3 color2 = this->s2->shadowed_color(position, prim, incoming, scene, light, num_shadow_rays);

        return this->combine_colors(color1, color2);
    }

    // return: the opacity of each shader mixed in the specified proportion.
    float transparency() const override {
        // TODO: This is a bit of a hack, is there a better way?
        vec3 a = vec3(s1->transparency(), 0, 0);
        vec3 b = vec3(s2->transparency(), 0, 0);
        return this->combine_colors(a, b).x;
    }

    /*
     ** Convenience functions for creating mix shaders using different functions.
     */

    // return: a mix shader that mixes both shaders with the given ratio.
    //         Shader s2 recieves proportion, and s1 recieves (1 - proportion).
    //
    static Mix *ratio(const Shader *s1, const Shader *s2, float proportion) {
        auto combine_colors = [=](vec3 col1, vec3 col2) {
            return glm::mix(col1, col2, proportion);
        };
        return new Mix(s1, s2, combine_colors);
    }

    // return: a shader that multiplies both shaders together.
    static Mix *multiply(const Shader *s1, const Shader *s2) {
        auto combine_colors = [=](vec3 col1, vec3 col2) {
            return col1 * col2;
        };
        return new Mix(s1, s2, combine_colors);
    }

    // return: a shader that adds both shaders together.
    static Mix *add(const Shader *s1, const Shader *s2) {
        auto combine_colors = [=](vec3 col1, vec3 col2) {
            return col1 + col2;
        };
        return new Mix(s1, s2, combine_colors);
    }
};

#endif // MIX_SHADER_H
