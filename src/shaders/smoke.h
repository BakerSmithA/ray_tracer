#include <glm/glm.hpp>
#include <math.h>
#include <functional>
#include <optional>

using glm::mix;
using glm::clamp;
using std::optional;

#ifndef SMOKE_H
#define SMOKE_H

// Models suspended particles.
class Smoke: public Shader {
public:
    const vec3 base_color;
    // returns the transparency of the smoke for the distance a light ray
    // travelled through it.
    const function<float(float)> transparency_for_dist;

    Smoke(vec3 base_color, function<float(float)> transparency_for_dist):
        base_color(base_color), transparency_for_dist(transparency_for_dist) {
    }

    // return: a color depending on how far the ray has to travel before
    //         exiting the smoke.
    vec3 color(const vec4 position, const Primitive *smoke_prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const {
        // Offset into the shape as the excluded primitive on scene.closest_intersection
        // cannot be used here. This is because the smoke may be made of one
        // primitive (e.g. sphere) and we need to check for self-intersections.
        Ray outgoing = Ray(position, incoming.dir, incoming.bounces_remaining - 1)
                      .offset(smoke_prim->normal_at(position), -0.001);

        // The object behind the smoke.
        optional<Intersection> behind_obj_i = scene.closest_intersection_excluding_obj(outgoing, smoke_prim->parent_obj);

        // The color of the object behind the smoke, or black if there is no
        // object behind.
        float dist_to_obj = std::numeric_limits<float>::max();
        vec3 behind_obj_col = vec3(0, 0, 0);
        if (behind_obj_i) {
            behind_obj_col = behind_obj_i->primitive->shader->shadowed_color(behind_obj_i->pos, behind_obj_i->primitive, outgoing, scene, light, num_shadow_rays);
            dist_to_obj = length(behind_obj_i->pos - position);
        }

        // How much of the behind object shows through depends on how much
        // smoke the ray had to travel through.

        // The maximum distance the ray can travel in the smoke.
        float max_smoke_dist = this->max_distance_in_smoke(position, smoke_prim->parent_obj, outgoing, scene);
        // The object may or may not be inside the smoke. If it is inside then
        // less smoke has been travelled through.
        float smoke_dist = std::min(dist_to_obj, max_smoke_dist);
        float t = this->transparency_for_dist(smoke_dist);
        return mix(this->base_color, behind_obj_col, t);
    }

    // return: the specular color, as smoke is not currently affected by shadows.
    vec3 shadowed_color(vec4 position, const Primitive *smoke_prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const {
        if (!incoming.can_bounce()) {
            return vec3(0, 0, 0);
        }
        return this->color(position, smoke_prim, incoming, scene, light, num_shadow_rays);
    }

    float transparency(vec4 position, const Primitive *prim, const Ray &shadow_ray, const Scene &scene) const {
        return 1.0f;
    }

private:
    // param position: the position of the first intersection with the smoke.
    // param outgoing: the ray generated after the first intersection.
    // param smoke_obj: the smoke object.
    // return: The distance the ray travels in the smoke before exiting.
    float max_distance_in_smoke(const vec4 position, const Object *smoke_obj, const Ray &outgoing, const Scene &scene) const {
        // Only consider primitives that are also part of the smoke object.
        auto is_excluded_prim = [=](const Primitive *testing_prim) {
            return testing_prim->parent_obj != smoke_obj;
        };
        optional<Intersection> smoke_exit = scene.closest_intersection(outgoing, is_excluded_prim);

        if (!smoke_exit.has_value()) {
            throw std::runtime_error("Ray must exit smoke");
        }

        return length(position - smoke_exit->pos);
    }
};

#endif
