#include <glm/glm.hpp>

using glm::mix;

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
    vec3 color(const vec4 position, const Primitive *smoke, const Ray &incoming, const Scene &scene, const Light &light) const override {
        if (!incoming.can_bounce()) {
            return vec3(0, 0, 0);
        }

        // The idea is to see how far the ray travels in the smoke, which is
        // used to calculate how much of any objects behind are visible.
        vec4 offset = smoke->compute_normal(position) * 0.001f;
        Ray outgoing = Ray(position - offset, incoming.dir, incoming.bounces_remaining - 1);

        // Only consider primitives that are also part of the smoke object.
        auto is_excluded_prim = [&](const Primitive *testing_prim) {
            return testing_prim->obj_tag != smoke->obj_tag;
        };
        unique_ptr<Intersection> smoke_exit = scene.closest_intersection(outgoing, is_excluded_prim);

        // If the ray never exited the smoke, this shouldn't happen.
        if (smoke_exit == nullptr) {
            return vec3(0, 0, 0);
        }

        // The object behind the smoke.
        unique_ptr<Intersection> behind_obj_i = scene.closest_intersection_excluding_obj(outgoing, smoke->obj_tag);

        // The color of the object behind the smoke, or black if there is no
        // object behind.
        vec3 behind_obj_col = vec3(0, 0, 0);
        if (behind_obj_i) {
            behind_obj_col = behind_obj_i->primitive->shader->shadowed_color(behind_obj_i->pos, behind_obj_i->primitive, outgoing, scene, light);
        }

        // The distance travelled in the smoke.
        float smoke_dist = length(position - smoke_exit->pos);

        return mix(behind_obj_col, this->base_color, smoke_dist * smoke_dist * smoke_dist * 5);
    }

    virtual float transparency() const {
        return 1.0f;
    }
};

#endif
