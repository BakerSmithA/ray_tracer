#pragma once

#include "dist_from_center.h"
#include <math.h>

// A shader which deflects light rays according to an inverse square
// relationship with distance to the center of the object.
class GravitationalLens: public DistFromCenter {
public:
    GravitationalLens(float strength, float max_dist):
        DistFromCenter(GravitationalLens::bent_ray_color(strength), max_dist) {
    }

    // return: 100% transparency, as this shader only deflects light, it does
    //         not block it.
    float transparency(vec4 position, const Primitive *prim, const Ray &shadow_ray, const Scene &scene) const {
        return 0.0f;
    }

private:
    static ConvertDistToColor bent_ray_color(float strength) {
        auto bent_ray = [=](float closest_dist, vec4 projection, vec4 diff, vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) {
            // The angle to deflect the ray towards the center of the lens.
            float angle = strength / (closest_dist * closest_dist);
            float clamped_angle = glm::clamp(angle, 0.0f, (float)M_PI);

            vec3 outgoing_dir_3d = deflected(vec3(incoming.dir), clamped_angle, vec3(projection), vec3(diff));
            vec4 outgoing_dir = project_to_4D(outgoing_dir_3d);
            // The number of bounces is reduced due to this interaction.
            Ray outgoing_ray = Ray(position, outgoing_dir, incoming.bounces_remaining - 1);

            optional<Intersection> i = scene.closest_intersection(outgoing_ray, prim);
            if (!i.has_value()) {
                return vec3(0, 0, 0);
            }

            return i->primitive->shader->shadowed_color(i->pos, i->primitive, outgoing_ray, scene, light, num_shadow_rays);
        };

        return bent_ray;
    }
};
