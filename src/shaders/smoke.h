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
    vec3 color(const vec4 position, const Primitive *smoke_prim, const Ray &incoming, const Scene &scene, const PointLight &light, const int num_shadow_rays) const override {
        // Offset into the shape as the excluded primitive on scene.closest_intersection
        // cannot be used here. This is because the smoke may be made of one
        // primitive (e.g. sphere) and we need to check for self-intersections.
        Ray outgoing = Ray(position, incoming.dir, incoming.bounces_remaining - 1)
                      .offset(smoke_prim->compute_normal(position), -0.001);

        // The object behind the smoke.
        unique_ptr<Intersection> behind_obj_i = scene.closest_intersection_excluding_obj(outgoing, smoke_prim->parent_obj);

        // The color of the object behind the smoke, or black if there is no
        // object behind.
        vec3 behind_obj_col = vec3(0, 0, 0);
        if (behind_obj_i) {
            behind_obj_col = behind_obj_i->primitive->shader->shadowed_color(behind_obj_i->pos, behind_obj_i->primitive, outgoing, scene, light, num_shadow_rays);
        }

        // How much of the behind object shows through depends on how much
        // smoke the ray had to travel through.
        float smoke_dist = this->distance_in_smoke(position, smoke_prim->parent_obj, outgoing, scene);
        float t = this->smoke_transparency(smoke_dist);
        return mix(behind_obj_col, this->base_color, t);
    }

    // return: the color of the intersected surface, taking shadows from the
    //         light into account. If no light makes it from the light to the
    //         position, the color of the position is black.
    virtual vec3 shadowed_color(vec4 position, const Primitive *smoke_prim, const Ray &incoming, const Scene &scene, const PointLight &light, const int num_shadow_rays) const {
        return this->color(position, smoke_prim, incoming, scene, light, num_shadow_rays);

        // if (!incoming.can_bounce()) {
        //     return vec3(0, 0, 0);
        // }
        //
        // Ray shadow_ray = light.shadow_ray_to(position).offset(smoke_prim->compute_normal(position), -0.001);
        //
        // float smoke_dist_to_light = this->distance_in_smoke(position, smoke_prim->obj_tag, shadow_ray, scene);
        // float t_light = this->smoke_transparency(smoke_dist_to_light);
        //
        // // The shadow also depends on the proportion of light that passes through
        // // with the primary ray.
        // Ray outgoing = Ray(position, incoming.dir, incoming.bounces_remaining - 1)
        //               .offset(smoke_prim->compute_normal(position), -0.001);
        // float smoke_dist_to_obj = this->distance_in_smoke(position, smoke_prim->obj_tag, outgoing, scene);
        // float t_obj = this->smoke_transparency(smoke_dist_to_obj);
        //
        // // For shorter distances in the smoke, more light makes it through.
        // // Therefore do 1 minus.
        // float m = 1 - (3.2 * (t_obj * t_light));
        //
        // vec3 col = this->color(position, smoke_prim, incoming, scene, light);
        // return mix(vec3(0, 0, 0), col, m);
    }

    virtual float transparency() const {
        return 0.8f;
    }

private:
    // return: the transparency of the smoke, which is a function of the
    //         distance the ray travels in the smoke.
    float smoke_transparency(float smoke_dist) const {
        return clamp(pow(smoke_dist, 3) * 5, 0.0, 1.0);
    }

    // param position: the position of the first intersection with the smoke.
    // param outgoing: the ray generated after the first intersection.
    // param smoke_obj: the smoke object.
    // return: The distance the ray travels in the smoke before exiting.
    float distance_in_smoke(const vec4 position, const Object *smoke_obj, const Ray &outgoing, const Scene &scene) const {
        // Only consider primitives that are also part of the smoke object.
        auto is_excluded_prim = [=](const Primitive *testing_prim) {
            return testing_prim->parent_obj != smoke_obj;
        };
        unique_ptr<Intersection> smoke_exit = scene.closest_intersection(outgoing, is_excluded_prim);

        if (smoke_exit == nullptr) {
            throw std::runtime_error("Ray must exit smoke");
        }

        return length(position - smoke_exit->pos);
    }
};

#endif
