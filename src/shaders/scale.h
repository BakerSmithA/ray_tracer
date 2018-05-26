#pragma once

// Scales the color of the supplied shader to be within the supplied minimum
// and maximum colors.
class ScaleShader: public Shader {
public:
    const Shader *shader;
    const vec3 min_col;
    const vec3 max_col;

    ScaleShader(const Shader *shader, const vec3 min_col, const vec3 max_col):
        shader(shader), min_col(min_col), max_col(max_col) {
    }

    ScaleShader(const Shader *shader, const float min, const float max):
        ScaleShader(shader, vec3(min, min, min), vec3(max, max, max)) {
    }

    // return: the color of the intersected surface, as illuminated by a
    //         specific light.
    vec3 shadowed_color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const override {
        vec3 t = this->shader->shadowed_color(position, prim, incoming, scene, light, num_shadow_rays);
        return glm::mix(this->min_col, this->max_col, t);
    }
};
