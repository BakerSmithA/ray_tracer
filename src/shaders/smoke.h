#include <glm/glm.hpp>
#include <math.h>

using glm::mix;
using glm::clamp;

#ifndef SMOKE_H
#define SMOKE_H

// Models suspended particles.
class Smoke: public Shader {
public:
    const vec3 base_color;

    Smoke(vec3 base_color): base_color(base_color) {
    }

    // return: a color depending on how far the ray has to travel before
    //         exiting the smoke.
    vec3 color(const vec4 position, const Primitive *smoke, const Ray &incoming, const Scene &scene, const PointLight &light) const override {
        if (!incoming.can_bounce()) {
            return vec3(0, 0, 0);
        }

        // The idea is to see how far the ray travels in the smoke, which is
        // used to calculate how much of any objects behind are visible.
        vec4 offset = smoke->compute_normal(position) * 0.001f;
        Ray outgoing = Ray(position - offset, incoming.dir, incoming.bounces_remaining - 1);

        float smoke_dist = distance_in_smoke(position, smoke->obj_tag, outgoing, scene);

        // The object behind the smoke.
        unique_ptr<Intersection> behind_obj_i = scene.closest_intersection_excluding_obj(outgoing, smoke->obj_tag);

        // The color of the object behind the smoke, or black if there is no
        // object behind.
        vec3 behind_obj_col = vec3(0, 0, 0);
        if (behind_obj_i) {
            behind_obj_col = behind_obj_i->primitive->shader->shadowed_color(behind_obj_i->pos, behind_obj_i->primitive, outgoing, scene, light);
        }

        // The distance travelled in the smoke.
        // float smoke_dist = length(position - smoke_exit->pos);
        float smoke_transparency = clamp(pow(smoke_dist, 2) * 2 - 0.1, 0.0, 1.0);

        return mix(behind_obj_col, this->base_color, smoke_transparency);
    }

    // // return: the color of the intersected surface, taking shadows from the
    // //         light into account. If the position is in shadow, black is
    // //         returned, otherwise the shader is used to calculate color.
    // virtual vec3 shadowed_color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const PointLight &light) const {
    //
    // }

    virtual float transparency() const {
        return 0.8f;
    }

private:
    // param position: the position of the first intersection with the smoke.
    // param outgoing: the ray generated after the first intersection.
    // param smoke_tag: the tag accociated with the smoke object.
    // return: The distance the ray travels in the smoke before exiting.
    float distance_in_smoke(const vec4 position, const int smoke_tag, const Ray &outgoing, const Scene &scene) const {
        // Only consider primitives that are also part of the smoke object.
        auto is_excluded_prim = [=](const Primitive *testing_prim) {
            return testing_prim->obj_tag != smoke_tag;
        };
        unique_ptr<Intersection> smoke_exit = scene.closest_intersection(outgoing, is_excluded_prim);

        if (smoke_exit == nullptr) {
            throw std::runtime_error("Ray must exit smoke");
        }

        return length(position - smoke_exit->pos);
    }
};

#endif
